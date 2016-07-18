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


static const struct coord hour[12] = {
    {64, 34}, {79, 38}, {90, 49}, {94, 64}, {90, 79}, {79, 90}, {64, 94}, {49, 90}, {38, 79}, {34, 64}, {38, 49}, {49, 38},
};



static const struct coord minsec[60] = {
    {64, 24}, {68, 24}, {72, 25}, {76, 26}, {80, 27}, {84, 29}, {88, 32}, {91, 34}, {94, 37}, {96, 40}, {99, 44}, {101, 48}, {102, 52}, {103, 56}, {104, 60},
{104, 64}, {104, 68}, {103, 72}, {102, 76}, {101, 80}, {99, 84}, {96, 88}, {94, 91}, {91, 94}, {88, 96}, {84, 99}, {80, 101}, {76, 102}, {72, 103}, {68, 104},
{64, 104}, {60, 104}, {56, 103}, {52, 102}, {48, 101}, {44, 99}, {40, 96}, {37, 94}, {34, 91}, {32, 88}, {29, 84}, {27, 80}, {26, 76}, {25, 72}, {24, 68}, {24, 64},
{24, 60}, {25, 56}, {26, 52}, {27, 48}, {29, 44}, {32, 40}, {34, 37}, {37, 34}, {40, 32}, {44, 29}, {48, 27}, {52, 26}, {56, 25}, {60, 24}, };


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

static void draw_msgdots(void * notyet)
{
    unsigned int i;
    const int disc_radius = 10;

    u8g_SetFont(&u8g, u8g_font_freedoomr10r);
    u8g_SetFontPosCenter(&u8g);

    for (i = 0; i < ARRAY_SIZE(nhour); i++) {
        u8g_DrawDisc(&u8g, nhour[i].x, nhour[i].y, disc_radius, U8G_DRAW_ALL);
    }
}

static void draw_hands(int hh, int mm, int ss)
{
    /* draw hour hand */
    u8g_DrawLine(&u8g, WIDTH/2,HEIGHT/2, hour[hh].x, hour[hh].y);

    /* draw minute hand */
    u8g_DrawLine(&u8g, WIDTH/2,HEIGHT/2, minsec[mm].x, minsec[mm].y);

    /* draw second hand */
    u8g_DrawLine(&u8g, WIDTH/2,HEIGHT/2, minsec[ss].x, minsec[ss].y);
    u8g_DrawDisc(&u8g, minsec[ss].x, minsec[ss].y, 3, U8G_DRAW_ALL);
}

void display_watch(unsigned char hour, unsigned char minute, unsigned char second)
{
    u8g_FirstPage(&u8g);

    do {
        int w;

        u8g_SetColorIndex(&u8g, 1);
        u8g_DrawBox(&u8g, 0, 0, WIDTH, HEIGHT);
        u8g_SetColorIndex(&u8g, 0);
        u8g_DrawDisc(&u8g, WIDTH/2, HEIGHT/2, HEIGHT/2 - 8, U8G_DRAW_ALL);
        u8g_SetColorIndex(&u8g, 1);
        draw_msgdots(NULL);
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
