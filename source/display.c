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

#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))
#define HEIGHT (200)
#define WIDTH (200)

extern u8g_dev_t xGDEP015OC1u8gDevice;

struct _cp {
    int x;
    int y;
    int len;
};

static u8g_t u8g;

/* generated with coordinate_calculator.ods */
static const struct coord {
    int x;
    int y;
} nhour[12] = {
    { 100, 25 },
    { 138, 35 },
    { 165, 63 },
    { 175, 100 },
    { 165, 138 },
    { 138, 165 },
    { 100, 175 },
    { 63, 165 },
    { 35, 138 },
    { 25, 100 },
    { 35, 63 },
    { 63, 35 },
};

void display_main(int num_messages)
{
    int w, il;
    char *l;
    u8g_FirstPage(&u8g);

    do {
        u8g_SetColorIndex(&u8g, 1);
        u8g_DrawBox(&u8g, 0, 0, WIDTH, HEIGHT);
        u8g_SetColorIndex(&u8g, 0);

        u8g_SetFont(&u8g, u8g_font_profont22r);
        u8g_SetFontPosCenter(&u8g);
        l = "This is";
        il = 20;
        w = u8g_GetStrWidth(&u8g, l);
        u8g_DrawStr(&u8g, WIDTH/2 - w/2, il, l);
        l = "your brain";
        il += 20;
        w = u8g_GetStrWidth(&u8g, l);
        u8g_DrawStr(&u8g, WIDTH/2 - w/2, il, l);
        l = "on e-ink.";
        il += 20;
        w = u8g_GetStrWidth(&u8g, l);
        u8g_DrawStr(&u8g, WIDTH/2 - w/2, il, l);
        l = "This is";
        il += 40;
        w = u8g_GetStrWidth(&u8g, l);
        u8g_DrawStr(&u8g, WIDTH/2 - w/2, il, l);
        l = "your brain";
        il += 20;
        w = u8g_GetStrWidth(&u8g, l);
        u8g_DrawStr(&u8g, WIDTH/2 - w/2, il, l);
        l = "on Kinetis.";
        il += 20;
        w = u8g_GetStrWidth(&u8g, l);
        u8g_DrawStr(&u8g, WIDTH/2 - w/2, il, l);

    } while (u8g_NextPage(&u8g));
}

void display_init()
{
    u8g_Init(&u8g, &xGDEP015OC1u8gDevice);
}

void display_off()
{
}

void display_on()
{
}
