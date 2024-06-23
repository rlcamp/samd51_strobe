#if __has_include(<component-version.h>)
/* as invoked by Makefile, regardless of cmsis-atmel version */
#include <component-version.h>
#include <samd51.h>
#else
/* as invoked by a certain ide, in case people want to use it to test modules in isolation */
#include <samd51/include/component-version.h>
#include <samd51/include/samd51.h>
#endif

#include "samd51_feather_m4_strobe.h"

int main(void) {
    /* explicitly disable usb if the bootloader left it enabled */
    USB->DEVICE.CTRLA.bit.ENABLE = 0;

    /* reduce idle power consumption since we are just going to sleep forever outside isr */
    PM->SLEEPCFG.bit.SLEEPMODE = PM_SLEEPCFG_SLEEPMODE_STANDBY_Val;

    strobe_start(0);

    while (1) __WFE();
}
