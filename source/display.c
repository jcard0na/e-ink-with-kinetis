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
#include "okio_bubble.h"
#include "qrcode.h"

#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))
#define HEIGHT (200)
#define WIDTH (200)

extern u8g_dev_t xGDEW0154T1u8gDevice;

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


static void draw_watch(void * notyet)
{
    unsigned int i, h, w;
    char s[3];

    u8g_SetFont(&u8g, u8g_font_freedoomr10r);
    u8g_SetFontPosCenter(&u8g);

    for (i = 0; i < ARRAY_SIZE(nhour); i++) {
        h = (i == 0) ? 12 : i;
        sprintf(s, "%d", h);
        w = u8g_GetStrWidth(&u8g, s);
        u8g_DrawStr(&u8g, nhour[i].x - w/2, nhour[i].y, s);
    }
}

static void draw_msgdots(int num)
{
    unsigned int i;
    const int disc_radius = 10;

    u8g_SetFont(&u8g, u8g_font_freedoomr10r);
    u8g_SetFontPosCenter(&u8g);

    for (i = 0; i < (num % (ARRAY_SIZE(nhour) + 1)); i++) {
        u8g_DrawDisc(&u8g, nhour[i].x, nhour[i].y, disc_radius, U8G_DRAW_ALL);
    }
}

void display_main(int num_messages)
{
    u8g_FirstPage(&u8g);

    do {
        u8g_SetColorIndex(&u8g, 1);
        u8g_DrawBox(&u8g, 0, 0, WIDTH, HEIGHT);
        u8g_SetColorIndex(&u8g, 0);
        u8g_DrawDisc(&u8g, WIDTH/2, HEIGHT/2, HEIGHT/2 - 8, U8G_DRAW_ALL);
        u8g_SetColorIndex(&u8g, 1);
        draw_msgdots(num_messages);
        u8g_DrawDisc(&u8g, WIDTH/2, HEIGHT/2, HEIGHT/2 - 40, U8G_DRAW_ALL);
        u8g_SetColorIndex(&u8g, 0);
        u8g_DrawDisc(&u8g, WIDTH/2, HEIGHT/2, HEIGHT/2 - 45, U8G_DRAW_ALL);
        draw_watch(NULL);
        u8g_SetColorIndex(&u8g, 1);

        u8g_DrawBitmap(&u8g, WIDTH/2 - 64/2, HEIGHT/2 - 64/2, 64/8, 64, okio_bubble_64);
        /*u8g_SetFont(&u8g, u8g_font_profont22r);*/
        /*u8g_SetFontPosCenter(&u8g);*/
        /*w = u8g_GetStrWidth(&u8g, "OKIO");*/
        /*u8g_DrawStr(&u8g, WIDTH/2 - w/2, HEIGHT/2, "OKIO");*/

        /*draw_hands(hour % 12, minute, second);*/
    } while (u8g_NextPage(&u8g));
}

void display_qrcode()
{
    u8g_FirstPage(&u8g);

    do {
        u8g_DrawBitmap(&u8g, 0, 0, WIDTH/8, HEIGHT, qrcode);
    } while (u8g_NextPage(&u8g));
}

void display_init()
{
    u8g_Init(&u8g, &xGDEW0154T1u8gDevice);
}

void display_off()
{
}

void display_on()
{
}
