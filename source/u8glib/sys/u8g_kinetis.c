/*

  u8g_bcm2073x.c

  Porting layer for Universal 8bit Graphics Library

  Copyright (c) 2014, Rdona Designs <javier.c@rdona.com>
  All rights reserved.
*/

#include "u8g_kinetis.h"

void u8g_Delay(uint16_t val)
{
  /*delay_micro_seconds(1000UL*(uint32_t)val);*/
}

void u8g_MicroDelay(void)
{
  /*delay_micro_seconds(1);*/
}

void u8g_10MicroDelay(void)
{
  /*delay_micro_seconds(10);*/
}

uint8_t u8g_com_hw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr)
{
  switch(msg)
  {
    case U8G_COM_MSG_STOP:
      break;

    case U8G_COM_MSG_INIT:
      break;

    case U8G_COM_MSG_ADDRESS:                     /* define cmd (arg_val = 0) or data mode (arg_val = 1) */
     break;

    case U8G_COM_MSG_CHIP_SELECT:
      break;

    case U8G_COM_MSG_RESET:
      break;

    case U8G_COM_MSG_WRITE_BYTE:
      break;

    case U8G_COM_MSG_WRITE_SEQ:
    case U8G_COM_MSG_WRITE_SEQ_P:
      break;
  }
  return 1;
}
