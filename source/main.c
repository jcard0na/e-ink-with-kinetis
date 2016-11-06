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

void setWakeupConfig()
{
	LPTMR_SetTimerPeriod(LPTMR0, (LPO_CLK_FREQ * WAKEUP_INTERVAL_S) - 1U);
	LPTMR_StartTimer(LPTMR0);
	LPTMR_EnableInterrupts(LPTMR0, kLPTMR_TimerInterruptEnable);
	LLWU_EnableInternalModuleInterruptWakup(LLWU, LLWU_LPTMR_IDX, true);
	NVIC_EnableIRQ(LLWU_IRQn);
}


/* Interrupt handers, defined in startup/startup_MKL17Z4.S */
void LLWU_IRQHandler(void)
{
    /* If wakeup by LPTMR. */
    if (LLWU_GetInternalWakeupModuleFlag(LLWU, LLWU_LPTMR_IDX))
    {
        LPTMR_DisableInterrupts(LPTMR0, kLPTMR_TimerInterruptEnable);
        LPTMR_ClearStatusFlags(LPTMR0, kLPTMR_TimerCompareFlag);
        LPTMR_StopTimer(LPTMR0);
    }
}

void LPTMR0_IRQHandler(void)
{
    if (kLPTMR_TimerInterruptEnable & LPTMR_GetEnabledInterrupts(LPTMR0))
    {
        LPTMR_DisableInterrupts(LPTMR0, kLPTMR_TimerInterruptEnable);
        LPTMR_ClearStatusFlags(LPTMR0, kLPTMR_TimerCompareFlag);
        LPTMR_StopTimer(LPTMR0);
    }
}


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
//    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    LPTMR_GetDefaultConfig(&lptmrConfig);
    lptmrConfig.prescalerClockSource = kLPTMR_PrescalerClock_1; /* Use LPO as clock source. */
    lptmrConfig.bypassPrescaler = true;
    LPTMR_Init(LPTMR0, &lptmrConfig);

    NVIC_EnableIRQ(LLWU_IRQn);
    NVIC_EnableIRQ(LPTMR0_IRQn);

    display_init();
    /*while (fn[i] != NULL) {*/
        /*fn[i++]();*/
    /*}*/

    i = 0;
    for(;;) { /* Infinite loop to avoid leaving the main function */
	    setWakeupConfig();
        display_counter(i++);
		SMC_SetPowerModeVlps(SMC);
    }
    wait();
}
