/*
 vim: et sw=4 ts=4:
 */
/**
 * This is template for main module created by New Kinetis SDK 2.x Project Wizard. Enjoy!
 **/

#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "u8glib/u8g.h"

static u8g_t u8g;
extern u8g_dev_t xGDEP015OC1u8gDevice;

/*!
 * @brief Application entry point.
 */
int main(void) {
  /* Init board hardware. */
  BOARD_InitPins();
  BOARD_BootClockRUN();
  BOARD_InitDebugConsole();

  u8g_Init(&u8g, &xGDEP015OC1u8gDevice);
  u8g_InitCom(&u8g, &xGDEP015OC1u8gDevice, 0);

  for(;;) { /* Infinite loop to avoid leaving the main function */
    __asm("NOP"); /* something to use as a breakpoint stop while looping */
  }
}
