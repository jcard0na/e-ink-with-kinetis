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



/*!
 * @brief Application entry point.
 */
int main(void)
{
    int i = 0;

    /* Init board hardware. */
    BOARD_InitPins();
    BOARD_BootClockVLPR();
//    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    display_init();
    while (fn[i] != NULL) {
        fn[i++]();
    }

    i = 0;
    for(;;) { /* Infinite loop to avoid leaving the main function */
        wait();
        wait();
        display_counter(i++);
    }
}
