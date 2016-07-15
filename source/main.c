/*
 vim: et sw=4 ts=4:
 */
/**
 * This is template for main module created by New Kinetis SDK 2.x Project Wizard. Enjoy!
 **/

#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "Display_EPD_W21.h"
#include "Display_EPD_W21_spi.h"
#include "photo.h"

const unsigned char LUTDefault_part[31] = {
        0x32,   // command

/*      0x00,0x00,0x00,0x18,
        0x18,0x18,0x18,0x00,
        0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,
        0x40,0x14,0x34,0x01,
        0x00,0x00,0x00,0x00,
        0x00,0x00,
*/
 0x10 //C221 25C partial update waveform
,0x18
,0x18
,0x08
,0x18
,0x18
,0x08
,0x00
,0x00
,0x00
,0x00
,0x00
,0x00
,0x00
,0x00
,0x00
,0x00
,0x00
,0x00
,0x00
,0x13
,0x14
,0x44
,0x12
,0x00
,0x00
,0x00
,0x00
,0x00
,0x00
};

const unsigned char LUTDefault_full[31] = {
        0x32,   // command
/*      0x11,0x11,0x10,0x02,    // full
        0x02,0x22,0x22,0x22,
        0x22,0x22,0x51,0x51,
        0x55,0x88,0x08,0x08,
    0x88,0x88,0x00,0x00,
        0x34,0x23,0x12,0x21,
        0x24,0x28,0x22,0x21,
        0xA1,0x01
*/
 0x02 //C221 25C Full update waveform
,0x02
,0x01
,0x11
,0x12
,0x12
,0x22
,0x22
,0x66
,0x69
,0x69
,0x59
,0x58
,0x99
,0x99
,0x88
,0x00
,0x00
,0x00
,0x00
,0xF8
,0xB4
,0x13
,0x51
,0x35
,0x51
,0x51
,0x19
,0x01
,0x00
};

void driver_delay_us(unsigned int xus)
{
        for(;xus>1;xus--);
}

void driver_delay_xms(unsigned long xms)
{
    unsigned long i = 0 , j=0;

    for(j=0;j<xms;j++)
        {
        for(i=0; i<256; i++);
    }
}

void parttest()
{
    unsigned int i,j,k;

    EPD_W21_WirteLUT((unsigned char *)LUTDefault_part);
    EPD_W21_POWERON();

    while(1)
    {
        part_display(0x00,0x18,0xc7,0x00,0x00,0x00);    // set ram
        EPD_W21_UpdataDisplay((unsigned char *)logo,200,200);
        EPD_W21_Update1();
        driver_delay_xms(100000);
        part_display(0x00,0x18,0xc7,0x00,0x00,0x00);    // set ram
        EPD_W21_UpdataDisplay((unsigned char *)logo,200,200);
        driver_delay_xms(10000);

        for(i=0;i<6;i++)
        {
            k=200-i*32-4;
            j=k-32;
            part_display(0x12,0x12,k%256,k/256,j%256,j/256);    // set ram
            EPD_W21_WriteDispRamMono(8, 32, 0x00);    // white
            EPD_W21_Update1();
            driver_delay_xms(20000);
            part_display(0x12,0x12,k%256,k/256,j%256,j/256);    // set ram
            EPD_W21_WriteDispRamMono(8, 32, 0x00);    // white
            driver_delay_xms(10000);
        }
    }
}


void EPD_init(void)
{
    EPD_W21_Init();            // display
    EPD_W21_WirteLUT((unsigned char *)LUTDefault_full);
    EPD_W21_POWERON();

    EPD_W21_SetRamPointer(0x00,0xC7,0x00);    // set ram
    EPD_W21_WriteDispRamMono(200, 200, 0x00);    // black
    EPD_W21_Update();
    driver_delay_xms(100000);

    EPD_W21_SetRamPointer(0x00,0xC7,0x00);    // set ram
    EPD_W21_WriteDispRam(200, 200, (unsigned char *)logo);    // white
    EPD_W21_Update();
    driver_delay_xms(100000);

    EPD_W21_SetRamPointer(0x00,0xC7,0x00);    // set ram
    EPD_W21_WriteDispRamMono(200, 200, 0xff);    // white
    EPD_W21_Update();
    driver_delay_xms(100000);

    parttest();
}

/*!
 * @brief Application entry point.
 */
int main(void) {
  /* Init board hardware. */
  BOARD_InitPins();
  BOARD_BootClockRUN();
  BOARD_InitDebugConsole();

  /* Add your code here */
  EPD_init();

  for(;;) { /* Infinite loop to avoid leaving the main function */
    __asm("NOP"); /* something to use as a breakpoint stop while looping */
  }
}
