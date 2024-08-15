/* attempts to provide workalike subset of functionality in adafruit samd core, which is in
 turn derived from cmsis. significant deviations should be noted

 ideally, this file should be able to be used in the following scenarios:
 - with a Makefile using upstream cmsis/atmel/gcc
 - with the same Makefile using older cmsis/atmel/gcc as packaged by adafruit
 - within a sketch in the arduino ide, using the adafruit samd core
 */

#if __has_include(<component-version.h>)
/* as invoked by Makefile, regardless of cmsis-atmel version */
#include <component-version.h>
#include <samd51.h>
#else
/* as invoked by a certain ide, in case people want to use it to test modules in isolation */
#include <samd51/include/component-version.h>
#include <samd51/include/samd51.h>
#endif

/* symbols provided by linker script, referred to within Reset_Handler and exception_table.
 deviation from cmsis: these are the symbol names provided by the adafruit linker script,
 with which we want to remain compatible */

extern unsigned char __etext[]; /* where data comes from in flash */
extern unsigned char __data_start__[], __data_end__[]; /* where data goes in RAM */
extern unsigned char __bss_start__[], __bss_end__[]; /* where bss goes in RAM */

/* execution nominally starts here on reset (actually when exiting bootloader) */
__attribute((noreturn)) void Reset_Handler(void) {
    /* copy data section from flash to sram */
    if ((uintptr_t)__etext != (uintptr_t)__data_start__)
        __builtin_memcpy(__data_start__, __etext, (uintptr_t)__data_end__ - (uintptr_t)__data_start__);

    /* clear the bss section in sram */
    __builtin_memset(__bss_start__, 0, (uintptr_t)__bss_end__ - (uintptr_t)__bss_start__);

    /* enable floating point and flush state */
    SCB->CPACR |= (0xFu << 20);
    __DSB();
    __ISB();

    /* deviation from upstream cmsis: do clock init before C or C++ constructors are run */
    SystemInit();

    /* deviation from adafruit samd core: run the constructors before main(), rather than
     doing both clock init and constructors in main() */
    extern void __libc_init_array();
    __libc_init_array();

    /* note that this can be the default arduino main but should NOT be the adafruit main,
     which expects to call the above two functions internally. if using this within adafruit
     core, you MUST also override main(), otherwise constructors will be called twice */
    extern int main(void);
    main();

    /* hopefully we never get here but if we do, just sleep forever */
    while (1) __WFI();
}

/* default dummy handler, hangs forever if encountered */
static void Dummy_Handler(void) { while (1) __WFI(); }

/* default empty handler, returns harmless if encountered */
static void Empty_Handler(void) { }

/* upstream cmsis calls this NonMaskableInt_Handler, TODO: support both symbol names? */
__attribute__ ((weak, alias("Dummy_Handler"))) void NMI_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void HardFault_Handler(void);
/* upstream cmsis calls this MemManagement_Handler, TODO: support both symbol names? */
__attribute__ ((weak, alias("Dummy_Handler"))) void MemManage_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void BusFault_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void UsageFault_Handler(void);
/* upstream cmsis calls this SVCall_Handler, TODO: support both symbol names? */
__attribute__ ((weak, alias("Dummy_Handler"))) void SVC_Handler(void);
/* upstream cmsis calls this DebugMonitor_Handler, TODO: support both symbol names? */
__attribute__ ((weak, alias("Dummy_Handler"))) void DebugMon_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void PendSV_Handler(void);

__attribute__ ((weak, alias("Empty_Handler"))) void SysTick_Handler(void);

/* above are standard cortex-m4 handlers, below are samd51-specific peripherals handlers */
__attribute__ ((weak, alias("Dummy_Handler"))) void PM_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void MCLK_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void OSCCTRL_0_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void OSCCTRL_1_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void OSCCTRL_2_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void OSCCTRL_3_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void OSCCTRL_4_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void OSC32KCTRL_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void SUPC_0_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void SUPC_1_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void WDT_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void RTC_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void EIC_0_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void EIC_1_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void EIC_2_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void EIC_3_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void EIC_4_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void EIC_5_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void EIC_6_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void EIC_7_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void EIC_8_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void EIC_9_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void EIC_10_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void EIC_11_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void EIC_12_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void EIC_13_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void EIC_14_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void EIC_15_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void FREQM_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void NVMCTRL_0_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void NVMCTRL_1_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void DMAC_0_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void DMAC_1_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void DMAC_2_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void DMAC_3_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void DMAC_4_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void EVSYS_0_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void EVSYS_1_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void EVSYS_2_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void EVSYS_3_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void EVSYS_4_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void PAC_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void TAL_0_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void TAL_1_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void RAMECC_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void SERCOM0_0_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void SERCOM0_1_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void SERCOM0_2_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void SERCOM0_3_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void SERCOM1_0_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void SERCOM1_1_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void SERCOM1_2_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void SERCOM1_3_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void SERCOM2_0_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void SERCOM2_1_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void SERCOM2_2_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void SERCOM2_3_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void SERCOM3_0_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void SERCOM3_1_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void SERCOM3_2_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void SERCOM3_3_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void SERCOM4_0_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void SERCOM4_1_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void SERCOM4_2_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void SERCOM4_3_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void SERCOM5_0_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void SERCOM5_1_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void SERCOM5_2_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void SERCOM5_3_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void SERCOM6_0_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void SERCOM6_1_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void SERCOM6_2_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void SERCOM6_3_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void SERCOM7_0_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void SERCOM7_1_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void SERCOM7_2_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void SERCOM7_3_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void CAN0_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void CAN1_Handler(void);

__attribute__ ((weak, alias("Empty_Handler"))) void USB_0_Handler(void);
__attribute__ ((weak, alias("Empty_Handler"))) void USB_1_Handler(void);
__attribute__ ((weak, alias("Empty_Handler"))) void USB_2_Handler(void);
__attribute__ ((weak, alias("Empty_Handler"))) void USB_3_Handler(void);

__attribute__ ((weak, alias("Dummy_Handler"))) void TCC0_0_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void TCC0_1_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void TCC0_2_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void TCC0_3_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void TCC0_4_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void TCC0_5_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void TCC0_6_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void TCC1_0_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void TCC1_1_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void TCC1_2_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void TCC1_3_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void TCC1_4_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void TCC2_0_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void TCC2_1_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void TCC2_2_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void TCC2_3_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void TCC3_0_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void TCC3_1_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void TCC3_2_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void TCC4_0_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void TCC4_1_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void TCC4_2_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void TC0_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void TC1_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void TC2_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void TC3_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void TC4_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void TC5_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void TC6_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void TC7_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void PDEC_0_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void PDEC_1_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void PDEC_2_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void ADC0_0_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void ADC0_1_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void ADC1_0_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void ADC1_1_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void AC_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void DAC_0_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void DAC_1_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void DAC_2_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void DAC_3_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void DAC_4_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void I2S_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void PCC_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void AES_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void TRNG_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void ICM_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void PUKCC_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void QSPI_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void SDHC0_Handler(void);
__attribute__ ((weak, alias("Dummy_Handler"))) void SDHC1_Handler(void);

/* deviation from upstream cmsis: this is in .isr_vector instead of .vectors */
__attribute__((used, section(".isr_vector"))) const DeviceVectors exception_table = {
    /* initial stack pointer */
    .pvStack = (void *)(uintptr_t)(HSRAM_ADDR + HSRAM_SIZE),

    /* cortex-m4 handlers */
    .pfnReset_Handler = (void *)Reset_Handler, /* this table entry defines where pc starts */
#if BUILD_NUMBER == 61
    .pfnNMI_Handler = (void *)NMI_Handler,
#else
    .pfnNonMaskableInt_Handler = (void *)NMI_Handler,
#endif
    .pfnHardFault_Handler = (void *)HardFault_Handler,
#if BUILD_NUMBER == 61
    .pfnMemManage_Handler = (void *)MemManage_Handler,
#else
    .pfnMemManagement_Handler = (void *)MemManage_Handler,
#endif
    .pfnBusFault_Handler = (void *)BusFault_Handler,
    .pfnUsageFault_Handler = (void *)UsageFault_Handler,
#if BUILD_NUMBER == 61
    .pfnSVC_Handler = (void *)SVC_Handler,
    .pfnDebugMon_Handler = (void *)DebugMon_Handler,
#else
    .pfnSVCall_Handler = (void *)SVC_Handler,
    .pfnDebugMonitor_Handler = (void *)DebugMon_Handler,
#endif
    .pfnPendSV_Handler = (void *)PendSV_Handler,
    .pfnSysTick_Handler = (void *)SysTick_Handler,

    /* samd51-specific peripheral handlers */
    .pfnPM_Handler = (void *)PM_Handler,
    .pfnMCLK_Handler = (void *)MCLK_Handler,
    .pfnOSCCTRL_0_Handler = (void *)OSCCTRL_0_Handler,
    .pfnOSCCTRL_1_Handler = (void *)OSCCTRL_1_Handler,
    .pfnOSCCTRL_2_Handler = (void *)OSCCTRL_2_Handler,
    .pfnOSCCTRL_3_Handler = (void *)OSCCTRL_3_Handler,
    .pfnOSCCTRL_4_Handler = (void *)OSCCTRL_4_Handler,
    .pfnOSC32KCTRL_Handler = (void *)OSC32KCTRL_Handler,
    .pfnSUPC_0_Handler = (void *)SUPC_0_Handler,
    .pfnSUPC_1_Handler = (void *)SUPC_1_Handler,
    .pfnWDT_Handler = (void *)WDT_Handler,
    .pfnRTC_Handler = (void *)RTC_Handler,
    .pfnEIC_0_Handler = (void *)EIC_0_Handler,
    .pfnEIC_1_Handler = (void *)EIC_1_Handler,
    .pfnEIC_2_Handler = (void *)EIC_2_Handler,
    .pfnEIC_3_Handler = (void *)EIC_3_Handler,
    .pfnEIC_4_Handler = (void *)EIC_4_Handler,
    .pfnEIC_5_Handler = (void *)EIC_5_Handler,
    .pfnEIC_6_Handler = (void *)EIC_6_Handler,
    .pfnEIC_7_Handler = (void *)EIC_7_Handler,
    .pfnEIC_8_Handler = (void *)EIC_8_Handler,
    .pfnEIC_9_Handler = (void *)EIC_9_Handler,
    .pfnEIC_10_Handler = (void *)EIC_10_Handler,
    .pfnEIC_11_Handler = (void *)EIC_11_Handler,
    .pfnEIC_12_Handler = (void *)EIC_12_Handler,
    .pfnEIC_13_Handler = (void *)EIC_13_Handler,
    .pfnEIC_14_Handler = (void *)EIC_14_Handler,
    .pfnEIC_15_Handler = (void *)EIC_15_Handler,
    .pfnFREQM_Handler = (void *)FREQM_Handler,
    .pfnNVMCTRL_0_Handler = (void *)NVMCTRL_0_Handler,
    .pfnNVMCTRL_1_Handler = (void *)NVMCTRL_1_Handler,
    .pfnDMAC_0_Handler = (void *)DMAC_0_Handler,
    .pfnDMAC_1_Handler = (void *)DMAC_1_Handler,
    .pfnDMAC_2_Handler = (void *)DMAC_2_Handler,
    .pfnDMAC_3_Handler = (void *)DMAC_3_Handler,
    .pfnDMAC_4_Handler = (void *)DMAC_4_Handler,
    .pfnEVSYS_0_Handler = (void *)EVSYS_0_Handler,
    .pfnEVSYS_1_Handler = (void *)EVSYS_1_Handler,
    .pfnEVSYS_2_Handler = (void *)EVSYS_2_Handler,
    .pfnEVSYS_3_Handler = (void *)EVSYS_3_Handler,
    .pfnEVSYS_4_Handler = (void *)EVSYS_4_Handler,
    .pfnPAC_Handler = (void *)PAC_Handler,

    .pfnRAMECC_Handler = (void *)RAMECC_Handler,
    .pfnSERCOM0_0_Handler = (void *)SERCOM0_0_Handler,
    .pfnSERCOM0_1_Handler = (void *)SERCOM0_1_Handler,
    .pfnSERCOM0_2_Handler = (void *)SERCOM0_2_Handler,
    .pfnSERCOM0_3_Handler = (void *)SERCOM0_3_Handler,
    .pfnSERCOM1_0_Handler = (void *)SERCOM1_0_Handler,
    .pfnSERCOM1_1_Handler = (void *)SERCOM1_1_Handler,
    .pfnSERCOM1_2_Handler = (void *)SERCOM1_2_Handler,
    .pfnSERCOM1_3_Handler = (void *)SERCOM1_3_Handler,
    .pfnSERCOM2_0_Handler = (void *)SERCOM2_0_Handler,
    .pfnSERCOM2_1_Handler = (void *)SERCOM2_1_Handler,
    .pfnSERCOM2_2_Handler = (void *)SERCOM2_2_Handler,
    .pfnSERCOM2_3_Handler = (void *)SERCOM2_3_Handler,
    .pfnSERCOM3_0_Handler = (void *)SERCOM3_0_Handler,
    .pfnSERCOM3_1_Handler = (void *)SERCOM3_1_Handler,
    .pfnSERCOM3_2_Handler = (void *)SERCOM3_2_Handler,
    .pfnSERCOM3_3_Handler = (void *)SERCOM3_3_Handler,
    .pfnSERCOM4_0_Handler = (void *)SERCOM4_0_Handler,
    .pfnSERCOM4_1_Handler = (void *)SERCOM4_1_Handler,
    .pfnSERCOM4_2_Handler = (void *)SERCOM4_2_Handler,
    .pfnSERCOM4_3_Handler = (void *)SERCOM4_3_Handler,
    .pfnSERCOM5_0_Handler = (void *)SERCOM5_0_Handler,
    .pfnSERCOM5_1_Handler = (void *)SERCOM5_1_Handler,
    .pfnSERCOM5_2_Handler = (void *)SERCOM5_2_Handler,
    .pfnSERCOM5_3_Handler = (void *)SERCOM5_3_Handler,
#ifdef ID_SERCOM6
    .pfnSERCOM6_0_Handler = (void *)SERCOM6_0_Handler,
    .pfnSERCOM6_1_Handler = (void *)SERCOM6_1_Handler,
    .pfnSERCOM6_2_Handler = (void *)SERCOM6_2_Handler,
    .pfnSERCOM6_3_Handler = (void *)SERCOM6_3_Handler,
#ifdef ID_SERCOM7
    .pfnSERCOM7_0_Handler = (void *)SERCOM7_0_Handler,
    .pfnSERCOM7_1_Handler = (void *)SERCOM7_1_Handler,
    .pfnSERCOM7_2_Handler = (void *)SERCOM7_2_Handler,
    .pfnSERCOM7_3_Handler = (void *)SERCOM7_3_Handler,
#endif
#endif

#ifdef ID_CAN0
    .pfnCAN0_Handler = (void *)CAN0_Handler,
#ifdef ID_CAN1
    .pfnCAN1_Handler = (void *)CAN1_Handler,
#endif
#endif

    .pfnUSB_0_Handler = (void *)USB_0_Handler,
    .pfnUSB_1_Handler = (void *)USB_1_Handler,
    .pfnUSB_2_Handler = (void *)USB_2_Handler,
    .pfnUSB_3_Handler = (void *)USB_3_Handler,

    .pfnTCC0_0_Handler = (void *)TCC0_0_Handler,
    .pfnTCC0_1_Handler = (void *)TCC0_1_Handler,
    .pfnTCC0_2_Handler = (void *)TCC0_2_Handler,
    .pfnTCC0_3_Handler = (void *)TCC0_3_Handler,
    .pfnTCC0_4_Handler = (void *)TCC0_4_Handler,
    .pfnTCC0_5_Handler = (void *)TCC0_5_Handler,
    .pfnTCC0_6_Handler = (void *)TCC0_6_Handler,
    .pfnTCC1_0_Handler = (void *)TCC1_0_Handler,
    .pfnTCC1_1_Handler = (void *)TCC1_1_Handler,
    .pfnTCC1_2_Handler = (void *)TCC1_2_Handler,
    .pfnTCC1_3_Handler = (void *)TCC1_3_Handler,
    .pfnTCC1_4_Handler = (void *)TCC1_4_Handler,
    .pfnTCC2_0_Handler = (void *)TCC2_0_Handler,
    .pfnTCC2_1_Handler = (void *)TCC2_1_Handler,
    .pfnTCC2_2_Handler = (void *)TCC2_2_Handler,
    .pfnTCC2_3_Handler = (void *)TCC2_3_Handler,
#ifdef ID_TCC3
    .pfnTCC3_0_Handler = (void *)TCC3_0_Handler,
    .pfnTCC3_1_Handler = (void *)TCC3_1_Handler,
    .pfnTCC3_2_Handler = (void *)TCC3_2_Handler,
#ifdef ID_TCC4
    .pfnTCC4_0_Handler = (void *)TCC4_0_Handler,
    .pfnTCC4_1_Handler = (void *)TCC4_1_Handler,
    .pfnTCC4_2_Handler = (void *)TCC4_2_Handler,
#endif
#endif

    .pfnTC0_Handler = (void *)TC0_Handler,
    .pfnTC1_Handler = (void *)TC1_Handler,
    .pfnTC2_Handler = (void *)TC2_Handler,
    .pfnTC3_Handler = (void *)TC3_Handler,
#ifdef ID_TC4
    .pfnTC4_Handler = (void *)TC4_Handler,
#ifdef ID_TC5
    .pfnTC5_Handler = (void *)TC5_Handler,
#ifdef ID_TC6
    .pfnTC6_Handler = (void *)TC6_Handler,
#ifdef ID_TC7
    .pfnTC7_Handler = (void *)TC7_Handler,
#endif
#endif
#endif
#endif
    .pfnPDEC_0_Handler = (void *)PDEC_0_Handler,
    .pfnPDEC_1_Handler = (void *)PDEC_1_Handler,
    .pfnPDEC_2_Handler = (void *)PDEC_2_Handler,
    .pfnADC0_0_Handler = (void *)ADC0_0_Handler,
    .pfnADC0_1_Handler = (void *)ADC0_1_Handler,
    .pfnADC1_0_Handler = (void *)ADC1_0_Handler,
    .pfnADC1_1_Handler = (void *)ADC1_1_Handler,
    .pfnAC_Handler = (void *)AC_Handler,
    .pfnDAC_0_Handler = (void *)DAC_0_Handler,
    .pfnDAC_1_Handler = (void *)DAC_1_Handler,
    .pfnDAC_2_Handler = (void *)DAC_2_Handler,
    .pfnDAC_3_Handler = (void *)DAC_3_Handler,
    .pfnDAC_4_Handler = (void *)DAC_4_Handler,
#ifdef ID_I2S
    .pfnI2S_Handler = (void *)I2S_Handler,
#endif
    .pfnPCC_Handler = (void *)PCC_Handler,
    .pfnAES_Handler = (void *)AES_Handler,
    .pfnTRNG_Handler = (void *)TRNG_Handler,
    .pfnICM_Handler = (void *)ICM_Handler,
    .pfnPUKCC_Handler = (void *)PUKCC_Handler,
    .pfnQSPI_Handler = (void *)QSPI_Handler,
    .pfnSDHC0_Handler = (void *)SDHC0_Handler,
#ifdef ID_SDHC1
    .pfnSDHC1_Handler = (void *)SDHC1_Handler,
#endif
};

/* everything below attempts to provide a useful subset of adafruit's startup environment */

/* deviation from adafruit/arduino: note that the adafruit core uses GCLKs 2 and 4 for 100
 and 1 MHz respectively, we don't, but we should probably not reuse those two GCLKs for
 other clock frequencies */

static void switch_cpu_to_32kHz(void) {
#ifdef CRYSTALLESS
    OSC32KCTRL->OSCULP32K.bit.EN32K = 1;
#else
    /* enable 32 kHz xtal oscillator */
    OSC32KCTRL->XOSC32K.reg = (OSC32KCTRL_XOSC32K_Type) { .bit = {
        .ENABLE = 1, .EN1K = 1, .EN32K = 1,
        .CGM = OSC32KCTRL_XOSC32K_CGM_XT_Val,
        .XTALEN = 1
    }}.reg;
    while (!OSC32KCTRL->STATUS.bit.XOSC32KRDY);
#endif

    /* reset gclk peripheral */
    GCLK->CTRLA.bit.SWRST = 1;
    while (GCLK->SYNCBUSY.bit.SWRST);

    /* one or the other of the 32 kHz oscillators will be generic clock generator 3 */
#ifndef CRYSTALLESS
    GCLK->GENCTRL[3].reg = (GCLK_GENCTRL_Type) { .bit = { .SRC = GCLK_GENCTRL_SRC_XOSC32K_Val, .GENEN = 1 }}.reg;
#else
    GCLK->GENCTRL[3].reg = (GCLK_GENCTRL_Type) { .bit = { .SRC = GCLK_GENCTRL_SRC_OSCULP32K_Val, .GENEN = 1 }}.reg;
#endif

    while (GCLK->SYNCBUSY.bit.GENCTRL3);

    /* temporarily use the ulp oscillator for generic clock 0 */
    GCLK->GENCTRL[0].reg = (GCLK_GENCTRL_Type) { .bit = { .SRC = GCLK_GENCTRL_SRC_OSCULP32K_Val, .GENEN = 1 }}.reg;
    while (GCLK->SYNCBUSY.reg & GCLK_SYNCBUSY_GENCTRL0);
}

static void switch_cpu_from_32kHz_to_fast(void) {
    /* bring up dfll in open loop mode */

    OSCCTRL->DFLLCTRLA.reg = 0;

    OSCCTRL->DFLLMUL.reg = (OSCCTRL_DFLLMUL_Type) { .bit = { .CSTEP = 0x1, .FSTEP = 0x1, .MUL = 0 }}.reg;
    while (OSCCTRL->DFLLSYNC.reg & OSCCTRL_DFLLSYNC_DFLLMUL);

    OSCCTRL->DFLLCTRLB.reg = 0;
    while (OSCCTRL->DFLLSYNC.reg & OSCCTRL_DFLLSYNC_DFLLCTRLB);

    OSCCTRL->DFLLCTRLA.bit.ENABLE = 1;
    while (OSCCTRL->DFLLSYNC.reg & OSCCTRL_DFLLSYNC_ENABLE);

    /* chip errata 2.8.3 workaround says to set dfllmul, then clear ctrlb to select open loop, then
     enable the dfll, then do this weird dfllval reload thing, then set dfllctrlb to final value */
    OSCCTRL->DFLLVAL.reg = OSCCTRL->DFLLVAL.reg;
    while (OSCCTRL->DFLLSYNC.bit.DFLLVAL);

    OSCCTRL->DFLLCTRLB.reg = (OSCCTRL_DFLLCTRLB_Type) { .bit = { .WAITLOCK = 1, .CCDIS = 1 }}.reg;
    while (!OSCCTRL->STATUS.bit.DFLLRDY);

    if (48000000 == F_CPU)
    /* use the 48 MHz clock for the cpu */
        GCLK->GENCTRL[0].reg = (GCLK_GENCTRL_Type) { .bit = { .SRC = GCLK_GENCTRL_SRC_DFLL_Val, .GENEN = 1 }}.reg;
    else {
        /* divide by 48 to get a 1 MHz clock for generic clock generator 5 */
        GCLK->GENCTRL[5].reg = (GCLK_GENCTRL_Type) { .bit = { .SRC = GCLK_GENCTRL_SRC_DFLL_Val, .GENEN = 1, .DIV = 48U }}.reg;
        while (GCLK->SYNCBUSY.bit.GENCTRL5);

        /* set up fdpll0 at F_CPU (120 MHz) */
        GCLK->PCHCTRL[OSCCTRL_GCLK_ID_FDPLL0].reg = (GCLK_PCHCTRL_Type) { .bit = { .GEN = GCLK_PCHCTRL_GEN_GCLK5_Val, .CHEN = 1 }}.reg;

        OSCCTRL->Dpll[0].DPLLRATIO.reg = (OSCCTRL_DPLLRATIO_Type) { .bit = { .LDRFRAC = 0x00, .LDR = (F_CPU - 500000) / 1000000 }}.reg;
        while (OSCCTRL->Dpll[0].DPLLSYNCBUSY.bit.DPLLRATIO);

        /* must use lbypass due to chip errata 2.13.1 */
        OSCCTRL->Dpll[0].DPLLCTRLB.reg = (OSCCTRL_DPLLCTRLB_Type) { .bit = { .REFCLK = OSCCTRL_DPLLCTRLB_REFCLK_GCLK_Val, . LBYPASS = 1 }}.reg;

        OSCCTRL->Dpll[0].DPLLCTRLA.reg = (OSCCTRL_DPLLCTRLA_Type) { .bit.ENABLE = 1 }.reg;
        while (OSCCTRL->Dpll[0].DPLLSTATUS.bit.CLKRDY == 0 || OSCCTRL->Dpll[0].DPLLSTATUS.bit.LOCK == 0);

        /* 48 MHz clock, required for usb and many other things */
        GCLK->GENCTRL[1].reg = (GCLK_GENCTRL_Type) { .bit = { .SRC = GCLK_GENCTRL_SRC_DFLL_Val, .GENEN = 1, .IDC = 1 }}.reg;
        while (GCLK->SYNCBUSY.reg & GCLK_SYNCBUSY_GENCTRL1);

        /* use the 120 MHz clock for the cpu */
        GCLK->GENCTRL[0].reg = (GCLK_GENCTRL_Type) { .bit = { .SRC = GCLK_GENCTRL_SRC_DPLL0_Val, .GENEN = 1, .IDC = 1 }}.reg;
    }

    while (GCLK->SYNCBUSY.reg & GCLK_SYNCBUSY_GENCTRL0);

    /* with no divider */
    MCLK->CPUDIV.reg = MCLK_CPUDIV_DIV_DIV1;
}

void SystemInit(void) {
    /* zero wait states */
    NVMCTRL->CTRLA.bit.RWS = 0;

    switch_cpu_to_32kHz();
    switch_cpu_from_32kHz_to_fast();

    /* use ldo regulator */
    SUPC->VREG.bit.SEL = 0;

    /* enable cache */
    __disable_irq();
    CMCC->CTRL.reg = 1;
    __enable_irq();

    /* shut off 32 kHz oscillators we're not using */
#ifdef CRYSTALLESS
    OSC32KCTRL->XOSC32K.reg = 0;
#else
    OSC32KCTRL->OSCULP32K.reg &= ~(OSC32KCTRL_OSCULP32K_EN32K | OSC32KCTRL_OSCULP32K_EN1K);
#endif

    /* deviation from adafruit/arduino: omitted debugging stuff */

    /* load ac calibration bias */
    AC->CALIB.reg = (AC_CALIB_Type) { .bit.BIAS0 = (*((uint32_t *)AC_FUSES_BIAS0_ADDR) & AC_FUSES_BIAS0_Msk) >> AC_FUSES_BIAS0_Pos }.reg;

    /* load adc calibration stuff */
    ADC0->CALIB.reg = (ADC_CALIB_Type) { .bit = {
        .BIASREFBUF = (*((uint32_t *)ADC0_FUSES_BIASREFBUF_ADDR) & ADC0_FUSES_BIASREFBUF_Msk) >> ADC0_FUSES_BIASREFBUF_Pos,
        .BIASR2R = (*((uint32_t *)ADC0_FUSES_BIASR2R_ADDR) & ADC0_FUSES_BIASR2R_Msk) >> ADC0_FUSES_BIASR2R_Pos,
        .BIASCOMP = (*((uint32_t *)ADC0_FUSES_BIASCOMP_ADDR) & ADC0_FUSES_BIASCOMP_Msk) >> ADC0_FUSES_BIASCOMP_Pos
    }}.reg;

    ADC1->CALIB.reg = (ADC_CALIB_Type) { .bit = {
        .BIASREFBUF = (*((uint32_t *)ADC1_FUSES_BIASREFBUF_ADDR) & ADC1_FUSES_BIASREFBUF_Msk) >> ADC1_FUSES_BIASREFBUF_Pos,
        .BIASR2R = (*((uint32_t *)ADC1_FUSES_BIASR2R_ADDR) & ADC1_FUSES_BIASR2R_Msk) >> ADC1_FUSES_BIASR2R_Pos,
        .BIASCOMP = (*((uint32_t *)ADC1_FUSES_BIASCOMP_ADDR) & ADC1_FUSES_BIASCOMP_Msk) >> ADC1_FUSES_BIASCOMP_Pos
    }}.reg;

    /* load usb calibration stuff */
    USB->DEVICE.PADCAL.reg = (USB_PADCAL_Type) { .bit = {
        .TRANSN = (*((uint32_t *)USB_FUSES_TRANSN_ADDR) & USB_FUSES_TRANSN_Msk) >> USB_FUSES_TRANSN_Pos,
        .TRANSP = (*((uint32_t *)USB_FUSES_TRANSP_ADDR) & USB_FUSES_TRANSP_Msk) >> USB_FUSES_TRANSP_Pos,
        .TRIM = (*((uint32_t *)USB_FUSES_TRIM_ADDR) & USB_FUSES_TRIM_Msk) >> USB_FUSES_TRIM_Pos
    }}.reg;

    /* explicitly disable usb just in case the bootloader left it enabled */
    USB->DEVICE.CTRLA.bit.ENABLE = 0;
}

/* silence compiler warning about no previous prototype */
extern void _init(void);

/* newlib calls this from within __libc_init_array */
__attribute((weak)) void _init(void) { }
