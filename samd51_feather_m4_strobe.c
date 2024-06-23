#include "samd51_feather_m4_strobe.h"

#if __has_include(<component-version.h>)
/* as invoked by Makefile, regardless of cmsis-atmel version */
#include <component-version.h>
#include <samd51.h>
#else
/* as invoked by a certain ide, in case people want to use it to test modules in isolation */
#include <samd51/include/component-version.h>
#include <samd51/include/samd51.h>
#endif

/* this function MUST live in .data, otherwise instruction timing after wake from deep sleep
 is nondeterministic, which usually results in the MSB being garbled */
__attribute__((noinline, section(".datafunc")))
static void single_ws2812_set_grb(const uint32_t grb) {
    /* 83 ns is the gcd of the 3 delays we need, at both CPU speeds we use */
#if F_CPU >= 120000000
#define NOPS_166NS "nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;"
#elif F_CPU == 48000000
#define NOPS_166NS "nop;nop;nop;nop;nop;nop;nop;nop;"
#endif

    __disable_irq();

    /* loop over bits in the 24-bit GRB triplet, msb first */
    for (uint32_t bit = 1U << 23; bit; bit >>=1)
        if (grb & bit) {
            /* raise pin */
            PORT->Group[1].OUTSET.reg = 1U << 3;

            /* datasheet says 700 +/- 150 ns, empirically > 500 ns */
            asm volatile(NOPS_166NS NOPS_166NS NOPS_166NS NOPS_166NS :::);

            /* lower pin */
            PORT->Group[1].OUTCLR.reg = 1U << 3;

            /* datasheet says 600 +/- 150 ns, empirically > 16 ns */
            asm volatile(NOPS_166NS NOPS_166NS NOPS_166NS :::);
        } else {
            PORT->Group[1].OUTSET.reg = 1U << 3;

            /* datasheet says 350 +/- 150 ns, empirically > 33 and < 500 ns */
            /* on the first call after waking from deep sleep, we can apparently be running
             slow here, so do fewer than the otherwise necessary nops */
            asm volatile(NOPS_166NS NOPS_166NS :::);

            PORT->Group[1].OUTCLR.reg = 1U << 3;

            /* datasheet says 800 +/- 150 ns, empirically > 550 ns */
            asm volatile(NOPS_166NS NOPS_166NS NOPS_166NS NOPS_166NS :::);
        }

    __enable_irq();
}

/* as a convenience, we can use the WS2812B to convey status by setting its value in between
 flashes to something other than just black */
static unsigned idle_grb = 0;

void TC3_Handler(void) {
    if (TC3->COUNT8.INTFLAG.bit.MC0) {
        /* clear flag so that interrupt doesn't re-fire */
        TC3->COUNT8.INTFLAG.reg = (TC_INTFLAG_Type){ .bit.MC0 = 1 }.reg;

        single_ws2812_set_grb(idle_grb);
    }
    else if (TC3->COUNT8.INTFLAG.bit.OVF) {
        /* clear flag so that interrupt doesn't re-fire */
        TC3->COUNT8.INTFLAG.reg = (TC_INTFLAG_Type){ .bit.OVF = 1 }.reg;

        single_ws2812_set_grb(0xFFFFFF);
    }
}

static void tc3_init(void) {
    /* assume GCLK3 is one or the other 32 kHz reference, and we need to make sure it is
     enabled and allowed to run in stdby */
#ifdef CRYSTALLESS
    OSC32KCTRL->OSCULP32K.bit.EN32K = 1;
#else
    OSC32KCTRL->XOSC32K.bit.EN32K = 1;
    OSC32KCTRL->XOSC32K.bit.RUNSTDBY = 1;
    while (!OSC32KCTRL->STATUS.bit.XOSC32KRDY);
#endif

    /* make sure the APB is enabled for TC3 */
    MCLK->APBBMASK.bit.TC3_ = 1;

    /* use the 32 kHz clock peripheral as the source for TC3 */
    GCLK->PCHCTRL[TC3_GCLK_ID].reg = (GCLK_PCHCTRL_Type) { .bit = {
        .GEN = GCLK_PCHCTRL_GEN_GCLK3_Val,
        .CHEN = 1
    }}.reg;
    while (GCLK->SYNCBUSY.reg);

    /* reset the TC3 peripheral */
    TC3->COUNT8.CTRLA.bit.SWRST = 1;
    while (TC3->COUNT8.SYNCBUSY.bit.SWRST);

    TC3->COUNT8.CTRLA.reg = (TC_CTRLA_Type) { .bit = {
        .MODE = TC_CTRLA_MODE_COUNT8_Val,
        .PRESCALER = TC_CTRLA_PRESCALER_DIV1024_Val, /* count at 32 per second */
        .RUNSTDBY = 1 /* run in stdby */
    }}.reg;

    /* count to 4 seconds and roll over */
    TC3->COUNT8.PER.reg = 127;

    /* also fire the interrupt handler when the counter reaches 1 */
    TC3->COUNT8.CC[0].reg = 1;
    while (TC3->COUNT8.SYNCBUSY.bit.COUNT);

    /* fire the interrupt handler when count equals 0 or CC0 */
    TC3->COUNT8.INTENSET.reg = (TC_INTENSET_Type) { .bit.MC0 = 1, .bit.OVF = 1 }.reg;
    NVIC_EnableIRQ(TC3_IRQn);

    /* enable the timer */
    while (TC3->COUNT8.SYNCBUSY.reg);
    TC3->COUNT8.CTRLA.bit.ENABLE = 1;
    while (TC3->COUNT8.SYNCBUSY.bit.ENABLE);
}

void strobe_start(unsigned idle_grb_input) {
    /* prepare pin for output, initially low */
    PORT->Group[1].DIRSET.reg = 1U << 3;
    PORT->Group[1].OUTCLR.reg = 1U << 3;

    idle_grb = idle_grb_input;

    /* safe to elide delay here because it will be one tc3 tick before the first write */
    tc3_init();
}
