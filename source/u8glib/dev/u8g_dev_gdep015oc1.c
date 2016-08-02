/*
 * Copyright 2016, Muchi Corporation <javier@okio.io>
 * vim: et sw=4 ts=4:
 */

#include <string.h>
#include "Display_EPD_W21.h"
#include "Display_EPD_W21_spi.h"
#include "../u8g.h"
#include "okio_logo_no_mouth.h"
#include "okio_logo_only_mouth.h"

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

// this is undocumented voodoo from the vendor for full screen update mode
const unsigned char lut_vcom0[] ={	0x0E	,0x14	,0x01	,0x0A	,0x06	,0x04	,0x0A	,0x0A	,0x0F	,0x03	,0x03	,0x0C	,0x06	,0x0A	,0x00	};
const unsigned char lut_w[] ={	0x0E	,0x14	,0x01	,0x0A	,0x46	,0x04	,0x8A	,0x4A	,0x0F	,0x83	,0x43	,0x0C	,0x86	,0x0A	,0x04	};
const unsigned char lut_b[] ={	0x0E	,0x14	,0x01	,0x8A	,0x06	,0x04	,0x8A	,0x4A	,0x0F	,0x83	,0x43	,0x0C	,0x06	,0x4A	,0x04	};
const unsigned char lut_g1[] ={	0x8E	,0x94	,0x01	,0x8A	,0x06	,0x04	,0x8A	,0x4A	,0x0F	,0x83	,0x43	,0x0C	,0x06	,0x0A	,0x04	};
const unsigned char lut_g2[] ={	0x8E	,0x94	,0x01	,0x8A	,0x06	,0x04	,0x8A	,0x4A	,0x0F	,0x83	,0x43	,0x0C	,0x06	,0x0A	,0x04	};
const unsigned char lut_vcom1[] ={	0x03	,0x1D	,0x01	,0x01	,0x08	,0x23	,0x37	,0x37	,0x01	,0x00	,0x00	,0x00	,0x00	,0x00	,0x00	};
const unsigned char lut_red0[] ={	0x83	,0x5D	,0x01	,0x81	,0x48	,0x23	,0x77	,0x77	,0x01	,0x00	,0x00	,0x00	,0x00	,0x00	,0x00	};
const unsigned char lut_red1[] ={	0x03	,0x1D	,0x01	,0x01	,0x08	,0x23	,0x37	,0x37	,0x01	,0x00	,0x00	,0x00	,0x00	,0x00	,0x00	};




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

static uint8_t expand_nibble(uint8_t byte, bool upper_nibble)
{
    uint8_t nib;
    uint8_t extended;
    if (upper_nibble)
        nib = (0xf0 & byte) >> 4;
    else
        nib = 0xf & byte;
    extended = (nib & 0x1)        | ((nib & 0x1) << 1) |
               ((nib & 0x2) << 1) | ((nib & 0x2) << 2) |
               ((nib & 0x4) << 2) | ((nib & 0x4) << 3) |
               ((nib & 0x8) << 3) | ((nib & 0x8) << 4);
    return extended;
}

static void Ultrachip(void)
{
	unsigned int i;
	for(i=0;i<5000;i++)
	{
		EPD_W21_WriteDATA(expand_nibble(okio_logo_no_mouth[i], true));
		EPD_W21_WriteDATA(expand_nibble(okio_logo_no_mouth[i], false));
	}
	driver_delay_xms(2);
}

static void Ultrachip_red(void)
{
    unsigned int i;
    for(i=0;i<5000;i++)
	{
		EPD_W21_WriteDATA(okio_logo_only_mouth[i]);
	}
	driver_delay_xms(2);
}

static void pic_display(void)
{
	EPD_W21_WriteCMD(0x10);			//¿ªÊ¼´«ÊäºÚ°×Í¼Ïñ
	Ultrachip();
	EPD_W21_WriteCMD(0x13);			//¿ªÊ¼´«ÊäºìÍ¼Ïñ
	Ultrachip_red();
}


static void lut_bw(void)
{
	unsigned int count;
	EPD_W21_WriteCMD(0x20);
	for(count=0;count<15;count++)
		{EPD_W21_WriteDATA(lut_vcom0[count]);}

	EPD_W21_WriteCMD(0x21);
	for(count=0;count<15;count++)
		{EPD_W21_WriteDATA(lut_w[count]);}

	EPD_W21_WriteCMD(0x22);
	for(count=0;count<15;count++)
		{EPD_W21_WriteDATA(lut_b[count]);}

	EPD_W21_WriteCMD(0x23);
	for(count=0;count<15;count++)
		{EPD_W21_WriteDATA(lut_g1[count]);}

	EPD_W21_WriteCMD(0x24);
	for(count=0;count<15;count++)
		{EPD_W21_WriteDATA(lut_g2[count]);}
}

static void lut_red(void)
{
	unsigned int count;
	EPD_W21_WriteCMD(0x25);
	for(count=0;count<15;count++)
		{EPD_W21_WriteDATA(lut_vcom1[count]);}

	EPD_W21_WriteCMD(0x26);
	for(count=0;count<15;count++)
		{EPD_W21_WriteDATA(lut_red0[count]); }

	EPD_W21_WriteCMD(0x27);
	for(count=0;count<15;count++)
		{EPD_W21_WriteDATA(lut_red1[count]); }
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

static void lcd_chkstatus(void)
{
    while(1)
    {
        if(isEPD_W21_BUSY==0) break;
    }
    driver_delay_xms(2000);
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

            EPD_W21_WriteCMD(0x01);
            EPD_W21_WriteDATA(0x07);            //ÉèÖÃ¸ßµÍµçÑ¹
            EPD_W21_WriteDATA(0x00);
            EPD_W21_WriteDATA(0x08);
            EPD_W21_WriteDATA(0x00);
            EPD_W21_WriteCMD(0x06);         //boostÉè¶¨
            EPD_W21_WriteDATA(0x07);
            EPD_W21_WriteDATA(0x07);
            EPD_W21_WriteDATA(0x07);
            EPD_W21_WriteCMD(0x04);         //ÉÏµç
            lcd_chkstatus();                //²é¿´Ð¾Æ¬×´Ì¬
            driver_delay_xms(300000);
            EPD_W21_WriteCMD(0X00);
            EPD_W21_WriteDATA(0xcf);                //Ñ¡Ôñ×î´ó·Ö±æÂÊ
            EPD_W21_WriteCMD(0X50);
            EPD_W21_WriteDATA(0x27);
            EPD_W21_WriteCMD(0x30);                 //PLLÉè¶¨
            EPD_W21_WriteDATA(0x39);
            EPD_W21_WriteCMD(0x61);                 //ÏñËØÉè¶¨
            EPD_W21_WriteDATA(0xC8);                //200ÏñËØ
            EPD_W21_WriteDATA(0x00);                //200ÏñËØ
            EPD_W21_WriteDATA(0xC8);
            EPD_W21_WriteCMD(0x82);                 //vcomÉè¶¨
            EPD_W21_WriteDATA(0x0E);
            lut_bw();
            lut_red();
            pic_display();
            EPD_W21_WriteCMD(0x12);
            lcd_chkstatus();
            driver_delay_xms(300000);         //wait for fresh display
            EPD_W21_WriteCMD(0X50);
            EPD_W21_WriteDATA(0x17);                //BD floating
            EPD_W21_WriteCMD(0x82);                 //to solve Vcom drop
            EPD_W21_WriteDATA(0x00);
            EPD_W21_WriteCMD(0x01);                 //power setting
            EPD_W21_WriteDATA(0x02);                //gate switch to external
            EPD_W21_WriteDATA(0x00);
            EPD_W21_WriteDATA(0x00);
            EPD_W21_WriteDATA(0x00);
            driver_delay_xms(1500);                 //delay 1.5S
            EPD_W21_WriteCMD(0X02);                 //power off
            driver_delay_xms(40000);

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
