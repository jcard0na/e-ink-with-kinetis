/*
 * Copyright 2014, Javier Cardona
 * All Rights Reserved.
 *
 * vim:set et sw=4 ts=4:
 */

#include "u8glib/u8g.h"
#include "string.h"
#include "stdio.h"
#include "display.h"
#include "screens_bits.h"
#include "screens.h"

#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))
#define HEIGHT (200)
#define WIDTH (200)

extern u8g_dev_t xGDEP015OC1u8gDevice;

static u8g_t u8g;

void display_screen(enum screen_index si)
{
    u8g_FirstPage(&u8g);

    do {
        u8g_SetColorIndex(&u8g, 1);
        u8g_DrawXBM(&u8g, screens[si].x, screens[si].y, screens[si].w, screens[si].h, screens[si].bits);
    } while (u8g_NextPage(&u8g));
}


void display_init()
{
    u8g_Init(&u8g, &xGDEP015OC1u8gDevice);
}
