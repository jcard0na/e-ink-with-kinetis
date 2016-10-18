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

static u8g_t u8g;

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
        l = "\"Like a";
        il = 40;
        w = u8g_GetStrWidth(&u8g, l);
        u8g_DrawStr(&u8g, WIDTH/2 - w/2, il, l);
        l = "drop of";
        il += 20;
        w = u8g_GetStrWidth(&u8g, l);
        u8g_DrawStr(&u8g, WIDTH/2 - w/2, il, l);
        l = "e-ink in";
        il += 20;
        w = u8g_GetStrWidth(&u8g, l);
        u8g_DrawStr(&u8g, WIDTH/2 - w/2, il, l);
        l = "a glass of";
        il += 20;
        w = u8g_GetStrWidth(&u8g, l);
        u8g_DrawStr(&u8g, WIDTH/2 - w/2, il, l);
        l = "e-milk\"";
        il += 20;
        w = u8g_GetStrWidth(&u8g, l);
        u8g_DrawStr(&u8g, WIDTH/2 - w/2, il, l);

        il += 55;
        u8g_DrawRBox(&u8g, 35, il - 15, 130, 30, 5);
        l = ".......";
        w = u8g_GetStrWidth(&u8g, l);
        u8g_SetColorIndex(&u8g, 1);
        u8g_DrawStr(&u8g, WIDTH/2 - w/2, il, l);
        u8g_SetColorIndex(&u8g, 0);

        u8g_DrawRFrame(&u8g, 3, 3, 194, 194, 5);

        for (il = 20; il < 180; il += 30) {
            u8g_DrawDisc(&u8g,  15, il, 4, U8G_DRAW_ALL);
            u8g_DrawDisc(&u8g, 185, il, 4, U8G_DRAW_ALL);
        }
    } while (u8g_NextPage(&u8g));
}

void display_init()
{
    u8g_Init(&u8g, &xGDEP015OC1u8gDevice);
}
