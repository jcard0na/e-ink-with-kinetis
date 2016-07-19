/*
 * Copyright 2016, Muchi Corporation <javier@okio.io>
 * vim: et sw=4 ts=4:
 */

#include <string.h>
#include "Display_EPD_W21.h"
#include "Display_EPD_W21_spi.h"
#include "../u8g.h"


/*
 * The GDEP015OC1 accepts 1-byte worth of pixels at a time, corresponding to 8
 * sequential pixels along the X (horizontal) axis.
 *
 * We will use a single page for the entire display.
 *
 * Useful info: https://github.com/olikraus/u8glib/wiki/tpictureloop
 * */
#include <stddef.h>


/* Supported options are 8, 32 or 200.  Larger sizes will require more memory
 * but update faster the display.
 */
#define ROWS_PER_PAGE 200
#define ROWS_PER_PAGE_IN_BYTES ((ROWS_PER_PAGE + 7) / 8)                    /* i.e. 25 */
#define GDEP015OC1_ROWS 200
#define GDEP015OC1_COLUMNS 200
#define GDEP015OC1_BYTES_PER_LINE ((GDEP015OC1_COLUMNS + 7) / 8)    /* i.e. 25 */

const unsigned char LUTDefault_part[31] = {
        0x32,   // command
    // this is undocumented voodoo from the vendor for partial screen update mode
    0x10, 0x18, 0x18, 0x08, 0x18, 0x18, 0x08, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x13, 0x14, 0x44, 0x12, 0x00, 0x00 ,0x00, 0x00, 0x00, 0x00
};

const unsigned char LUTDefault_full[31] = {
        0x32,   // command
    // this is undocumented voodoo from the vendor for full screen update mode
    0x02, 0x02, 0x01, 0x11, 0x12, 0x12, 0x22, 0x22, 0x66, 0x69,
    0x69, 0x59, 0x58, 0x99, 0x99, 0x88, 0x00, 0x00, 0x00, 0x00,
    0xF8, 0xB4, 0x13, 0x51, 0x35, 0x51, 0x51, 0x19, 0x01, 0x00
};




static uint8_t cgram_[ROWS_PER_PAGE * GDEP015OC1_BYTES_PER_LINE];

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

static int update_lines (int start_line, int num_lines, const uint8_t * line_data)
{
#if (ROWS_PER_PAGE != GDEP015OC1_ROWS)
	int line = start_line;
    int end_line;

    if (line < 0 || num_lines != ROWS_PER_PAGE) {
        return -1;
    }

    end_line = start_line + num_lines - 1;

    part_display(0x00, 0x18, line, 0x00, end_line, 0x00);    // set ram
    EPD_W21_WriteDispRam(GDEP015OC1_COLUMNS, ROWS_PER_PAGE, (uint8_t *) line_data);
    EPD_W21_Update1();
    driver_delay_xms(10000);
    part_display(0x00, 0x18, line, 0x00, end_line, 0x00);    // set ram
    EPD_W21_WriteDispRam(GDEP015OC1_COLUMNS, ROWS_PER_PAGE, (uint8_t *) line_data);
#else
    EPD_W21_SetRamPointer(0x00, 0x00, 0x00);  // set ram
    EPD_W21_WriteDispRam(GDEP015OC1_COLUMNS, GDEP015OC1_ROWS, (uint8_t *) line_data);
    EPD_W21_Update();
#endif

    driver_delay_xms(1000);
    return 0;
}



static uint8_t u8g_com_fn (u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr)
{
    uint8_t rc = 0;

    switch (msg) {
        case U8G_COM_MSG_INIT:
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
            break;
        case U8G_DEV_MSG_GET_MODE:
            break;
        case U8G_DEV_MSG_INIT: {
            EPD_W21_Init();            // display
            EPD_W21_WirteLUT((unsigned char *)LUTDefault_full);
            EPD_W21_POWERON();

            EPD_W21_SetRamPointer(0x00, 0x00, 0x00);  // set ram
            EPD_W21_WriteDispRamMono(GDEP015OC1_COLUMNS, GDEP015OC1_ROWS, 0x00);
            EPD_W21_Update();
            driver_delay_xms(100000);

            EPD_W21_SetRamPointer(0x00, 0x00, 0x00);  // set ram
            EPD_W21_WriteDispRamMono(GDEP015OC1_COLUMNS, GDEP015OC1_ROWS, 0xff);
            EPD_W21_Update();
            driver_delay_xms(100000);

#if (ROWS_PER_PAGE != GDEP015OC1_ROWS)
            /* Switch to partial update mode */
            EPD_W21_WirteLUT((unsigned char *)LUTDefault_part);
            EPD_W21_POWERON();
#endif

            memset(cgram_, 0, sizeof(cgram_));
            break;
        }
        case U8G_DEV_MSG_PAGE_NEXT: {
            /* this function just delegates to U8G_COM_MSG_WRITE_SEQ */
            rc = u8g_pb_WriteBuffer(&pb, u8g, dev);
            if (rc == 0)
                return 0;
            break;
        }
    }
#if (ROWS_PER_PAGE == 8)
    return u8g_dev_pb8h1_base_fn(u8g, dev, msg, arg);
#elif ROWS_PER_PAGE == 32
    return u8g_dev_pb32h1_base_fn(u8g, dev, msg, arg);
#elif ROWS_PER_PAGE == 64
    return u8g_dev_pb200h1_base_fn(u8g, dev, msg, arg);
#elif ROWS_PER_PAGE == 200
    return u8g_dev_pb200h1_base_fn(u8g, dev, msg, arg);
#else
    #error "Unsupported ROWS_PER_PAGE"
#endif
}

u8g_dev_t xGDEP015OC1u8gDevice = { u8g_dev_fn, &pb, u8g_com_fn };
