/*
 * Copyright 2016, Muchi Corporation <javier@okio.io>
 * vim: et sw=4 ts=4:
 */

#include <string.h>
#include "Display_EPD_W21.h"
#include "Display_EPD_W21_spi.h"
#include "photo.h"
#include "../u8g.h"


/*
 * The GDEP015OC1 accepts 1-byte worth of pixels at a time, corresponding to 8
 * sequential pixels along the X (horizontal) axis.
 *
 * We divide the display into 4 pages of 200x50 pixels.
 * Therefore each page will be (200x50/8 =) 1250 bytes in size.
 *
 * Useful info: https://github.com/olikraus/u8glib/wiki/tpictureloop
 * */
#include <stddef.h>


#define ROWS_PER_PAGE 50
#define GDEP015OC1_ROWS 200
#define GDEP015OC1_COLUMNS 200
#define GDEP015OC1_BYTES_PER_LINE ((GDEP015OC1_COLUMNS + 7) / 8)    /* i.e. 25 */

const unsigned char LUTDefault_part[31] = {
        0x32,   // command
    // this is undocumented voodoo from the vendor...
    0x10, 0x18, 0x18, 0x08, 0x18, 0x18, 0x08, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x13, 0x14, 0x44, 0x12, 0x00, 0x00 ,0x00, 0x00, 0x00, 0x00
};



static uint8_t cgram_[ROWS_PER_PAGE * GDEP015OC1_BYTES_PER_LINE];    /* i.e. 1250 bytes */

static u8g_pb_t pb = {
    { /* u8g_page_t */
        ROWS_PER_PAGE,
        GDEP015OC1_ROWS,
        0, 0, 0
    },
    GDEP015OC1_COLUMNS,
    cgram_,
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

static uint8_t reverse_bits (uint8_t i)
{
  uint8_t rv;
  rv = i;
  rv = ((rv & 0xaa) >> 1) | ((rv & 0x55) << 1);
  rv = ((rv & 0xcc) >> 2) | ((rv & 0x33) << 2);
  rv = ((rv & 0xf0) >> 4) | ((rv & 0x0f) << 4);
  return rv;
}


static int update_lines (int start_line, int num_lines, const uint8_t * line_data)
{
    int line = start_line;
    int end_line;

    if (line < 0 || num_lines != ROWS_PER_PAGE) {
        return -1;
    }

    end_line = start_line + num_lines - 1;

    /*
     Select a page to update.  The x coordinate is in byte units
     (0 to 200/8 - 1) = 0 - 0x18
     The y coordinate is in pixels, 0-199 = 0-0xc7
     The second value for y is the most significant byte, and is always
     zero for this small display

     <--- 0x18 --->
          ^
          |
          |
         0xc7
          |
          |
          v
    */

    part_display(0x00, 0x18, end_line, 0x00, line, 0x00);    // set ram
    /*EPD_W21_WriteDispRam(GDEP015OC1_COLUMNS, ROWS_PER_PAGE, (uint8_t *) line_data);*/
    EPD_W21_WriteDispRamMono(GDEP015OC1_COLUMNS, ROWS_PER_PAGE, line);
    EPD_W21_Update1();
    driver_delay_xms(100000);
    part_display(0x00, 0x18, end_line, 0x00, line, 0x00);    // set ram
    /*EPD_W21_WriteDispRam(GDEP015OC1_COLUMNS, ROWS_PER_PAGE, (uint8_t *) line_data);*/
    EPD_W21_WriteDispRamMono(GDEP015OC1_COLUMNS, ROWS_PER_PAGE, line);
    driver_delay_xms(10000);
/*
    spi_cs_assert(DISPLAY);

    while (line < end_line) {
        cmd[1] = reverse_bits(line);
        __spi_write_buf(cmd, sizeof(cmd));
        __spi_write_buf(dp, GDEP015OC1_BYTES_PER_LINE);
        dp += GDEP015OC1_BYTES_PER_LINE;
        ++line;
        cmd[0] = 0;
    }
    cmd[1] = 0;
    __spi_write_buf(cmd, sizeof(cmd));
    spi_cs_deassert(DISPLAY);
*/

    return 0;
}



static uint8_t u8g_com_fn (u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr)
{
    uint8_t rc = 0;
    int i, j, k;

    switch (msg) {
        case U8G_COM_MSG_INIT:
            EPD_W21_Init();            // display
            EPD_W21_WirteLUT((unsigned char *)LUTDefault_part);
            EPD_W21_POWERON();

            // draw blinds
            for(i = 0;i < (GDEP015OC1_ROWS/ROWS_PER_PAGE); i++)
            {
                k=GDEP015OC1_COLUMNS-i*ROWS_PER_PAGE-4;
                j=k-ROWS_PER_PAGE;
                part_display(0x0, 0x18, k, 0, j, 0);    // set ram
                EPD_W21_WriteDispRamMono(GDEP015OC1_COLUMNS, ROWS_PER_PAGE, 0xff);    // white
                EPD_W21_Update1();
                driver_delay_xms(20000);
                /* confirmed experimentally: partial updates need to be written twice */
                part_display(0x0, 0x18, k, 0, j, 0);    // set ram
                EPD_W21_WriteDispRamMono(GDEP015OC1_COLUMNS, ROWS_PER_PAGE, 0xff);    // white
                driver_delay_xms(10000);
            }

            // Followed by logo
            part_display(0x00,0x18,0xc7,0x00,0x00,0x00);    // set ram
            EPD_W21_WriteDispRam(200, 200, (unsigned char *)logo);
            EPD_W21_Update1();
            driver_delay_xms(100000);
            part_display(0x00,0x18,0xc7,0x00,0x00,0x00);    // set ram
            EPD_W21_WriteDispRam(200, 200, (unsigned char *)logo);
            driver_delay_xms(10000);
            break;


        case U8G_COM_MSG_WRITE_SEQ: {
            int start_line = pb.p.page_y0;
            int num_lines = (pb.p.page_y1 - pb.p.page_y0) + 1;

            rc = ! update_lines(start_line, num_lines, cgram_);
            break;
            }
        /* All these messages below are not needed */
        case U8G_COM_MSG_STOP:
        case U8G_COM_MSG_ADDRESS:
        case U8G_COM_MSG_CHIP_SELECT:
        case U8G_COM_MSG_RESET:
        case U8G_COM_MSG_WRITE_BYTE:
        case U8G_COM_MSG_WRITE_SEQ_P:
        default:
            assert(-1);
            break;
        }
    return rc;
}

static uint8_t u8g_dev_fn (u8g_t * u8g, u8g_dev_t * dev, uint8_t msg, void * arg)
{
    int rc = 0;

    switch (msg) {
        default:
            /* Anything not specifically handled is delegated to the base function */
            rc = u8g_dev_pb8h1_base_fn(u8g, dev, msg, arg);
            break;
        case U8G_DEV_MSG_GET_MODE:
            rc = u8g_dev_pb8h1_base_fn(u8g, dev, msg, arg);
            break;
        case U8G_DEV_MSG_INIT: {
            memset(cgram_, 0, sizeof(cgram_));
            break;
        }
        case U8G_DEV_MSG_PAGE_NEXT: {
            /* this function just delegates to U8G_COM_MSG_WRITE_SEQ */
            rc = u8g_pb_WriteBuffer(&pb, u8g, dev);
            if (rc) {
                rc = u8g_dev_pb8h1_base_fn(u8g, dev, msg, arg);
            }
            break;
        }
    }
    return rc;
}

u8g_dev_t xGDEP015OC1u8gDevice = { u8g_dev_fn, &pb, u8g_com_fn };
