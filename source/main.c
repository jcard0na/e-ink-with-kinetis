/*
 vim: et sw=4 ts=4:
 */
/**
 * This is template for main module created by New Kinetis SDK 2.x Project Wizard. Enjoy!
 **/

#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "display.h"
#include "fsl_smc.h"
#include "fsl_lptmr.h"
#include "fsl_llwu.h"
#include "fsl_port.h"

#include "screens.h"


static void wait()
{
    long int cycles = CLOCK_GetFreq(kCLOCK_CoreSysClk);
    while (cycles--)
    {
        __NOP();
    }
}

void (*fn[])() = {
                   display_frames3,
                   /*display_circles1,*/   // This screen uses DrawDisc() which is very slow, skip
                   display_circles2,
                   /*display_circles3,*/   // DrawDisk(), skip
                   display_ellipses1,
                   display_frames1,
                   display_frames2,
                   display_main,
                   NULL
};

#define LLWU_LPTMR_IDX 0U      /* LLWU_M0IF */
#define WAKEUP_INTERVAL_S	10
#define LLWU_WAKEUP_PIN_IDX 14U /* LLWU_P14 */
#define LLWU_WAKEUP_PIN_TYPE kLLWU_ExternalPinRisingEdge

#define APP_WAKEUP_BUTTON_PORT PORTD
#define APP_WAKEUP_BUTTON_GPIO_PIN 4U
#define APP_WAKEUP_BUTTON_IRQ_HANDLER PORTCD_IRQHandler
#define APP_WAKEUP_BUTTON_IRQ_TYPE kPORT_InterruptRisingEdge

#define ENABLE_PERIODIC_WAKEUP 0
#define ENABLE_GPIO_WAKEUP 1

void setWakeupConfig()
{

#if ENABLE_PERIODIC_WAKEUP
    LPTMR_SetTimerPeriod(LPTMR0, (LPO_CLK_FREQ * WAKEUP_INTERVAL_S) - 1U);
    LPTMR_StartTimer(LPTMR0);
    LPTMR_EnableInterrupts(LPTMR0, kLPTMR_TimerInterruptEnable);
	LLWU_EnableInternalModuleInterruptWakup(LLWU, LLWU_LPTMR_IDX, true);
#endif /* ENABLE_PERIODIC_WAKEUP */

#if ENABLE_GPIO_WAKEUP
    PORT_SetPinInterruptConfig(APP_WAKEUP_BUTTON_PORT, APP_WAKEUP_BUTTON_GPIO_PIN, APP_WAKEUP_BUTTON_IRQ_TYPE);
    LLWU_SetExternalWakeupPinMode(LLWU, LLWU_WAKEUP_PIN_IDX, LLWU_WAKEUP_PIN_TYPE);
#endif /* ENABLE_GPIO_WAKEUP */
}

#if ENABLE_GPIO_WAKEUP
void APP_WAKEUP_BUTTON_IRQ_HANDLER(void)
{
    if ((1U << APP_WAKEUP_BUTTON_GPIO_PIN) & PORT_GetPinsInterruptFlags(APP_WAKEUP_BUTTON_PORT))
    {
        PORT_ClearPinsInterruptFlags(APP_WAKEUP_BUTTON_PORT, (1U << APP_WAKEUP_BUTTON_GPIO_PIN));
    }
}
#endif /* ENABLE_GPIO_WAKEUP */

#if ENABLE_PERIODIC_WAKEUP
void LPTMR0_IRQHandler(void)
{
    if (kLPTMR_TimerInterruptEnable & LPTMR_GetEnabledInterrupts(LPTMR0))
    {
        LPTMR_DisableInterrupts(LPTMR0, kLPTMR_TimerInterruptEnable);
        LPTMR_ClearStatusFlags(LPTMR0, kLPTMR_TimerCompareFlag);
        LPTMR_StopTimer(LPTMR0);
    }
}
#endif /* ENABLE_PERIODIC_WAKEUP */


/*!
 * @brief Application entry point.
 */
int main(void)
{
    int i = 0;
    lptmr_config_t lptmrConfig;


    /* Init board hardware. */
    BOARD_InitPins();
    BOARD_BootClockVLPR();
    BOARD_InitDebugConsole();

#ifdef ENABLE_PERIODIC_WAKEUP
    LPTMR_GetDefaultConfig(&lptmrConfig);
    lptmrConfig.prescalerClockSource = kLPTMR_PrescalerClock_1; /* Use LPO as clock source. */
    lptmrConfig.bypassPrescaler = true;
    LPTMR_Init(LPTMR0, &lptmrConfig);
    NVIC_EnableIRQ(LPTMR0_IRQn);
#endif /* ENABLE_PERIODIC_WAKEUP */

#ifdef ENABLE_GPIO_WAKEUP
    NVIC_EnableIRQ(PORTC_PORTD_IRQn);
#endif /* ENABLE_GPIO_WAKEUP */

    display_init();
    /*while (fn[i] != NULL) {*/
        /*fn[i++]();*/
    /*}*/

    i = 0;
    for(;;) { /* Infinite loop to avoid leaving the main function */
	    setWakeupConfig();
        display_screen(i++ % 6);
        // In this state the LLWM is disabled.  We will wake up via the regular interrupts (timer or gpio)
		SMC_SetPowerModeVlps(SMC);
        //SMC_SetPowerModeLls(SMC);
    }
    wait();
}
