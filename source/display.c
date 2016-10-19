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

void display_main()
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
        l = "Well Done";
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

void display_circles1()
{
    int i, pen;
    u8g_FirstPage(&u8g);

    do {
        u8g_SetColorIndex(&u8g, 1);
        u8g_DrawBox(&u8g, 0, 0, WIDTH, HEIGHT);
        pen = 0;
        u8g_SetColorIndex(&u8g, pen);

        for (i = 100; i > 0; i -= 4) {
            u8g_DrawDisc(&u8g,  100, 100, i, U8G_DRAW_ALL);
            pen = !pen;
            u8g_SetColorIndex(&u8g, pen);
        }
    } while (u8g_NextPage(&u8g));
}

void display_circles2()
{
    int i, pen;
    u8g_FirstPage(&u8g);

    do {
        u8g_SetColorIndex(&u8g, 0);
        u8g_DrawBox(&u8g, 0, 0, WIDTH, HEIGHT);
        pen = 1;
        u8g_SetColorIndex(&u8g, pen);

        for (i = 1; i < 100; i += 4) {
            u8g_DrawCircle(&u8g,  100, 100, i, U8G_DRAW_ALL);
        }
    } while (u8g_NextPage(&u8g));
}

void display_ellipses1()
{
    int j, k, pen;
    u8g_FirstPage(&u8g);

    do {
        u8g_SetColorIndex(&u8g, 0);
        u8g_DrawBox(&u8g, 0, 0, WIDTH, HEIGHT);
        pen = 1;
        u8g_SetColorIndex(&u8g, pen);

        for (j = 10; j < 180; j += 20)
            for (k = 10; k < 180; k += 20)
                    // This function only works with small ellipses of approx 20px radius
                    u8g_DrawEllipse(&u8g,  j, k, j/10, k/10, U8G_DRAW_ALL);

    } while (u8g_NextPage(&u8g));
}

void display_frames1()
{
    int i, pen;
    u8g_FirstPage(&u8g);

    do {
        u8g_SetColorIndex(&u8g, 0);
        u8g_DrawBox(&u8g, 0, 0, WIDTH, HEIGHT);
        pen = 1;
        u8g_SetColorIndex(&u8g, pen);

        for (i = 0; i < 200; i += 4) {
            u8g_DrawFrame(&u8g,  i, i, 200 - i, 200 - i);
        }

    } while (u8g_NextPage(&u8g));
}

void display_frames2()
{
    int i, pen;
    u8g_FirstPage(&u8g);

    do {
        u8g_SetColorIndex(&u8g, 1);
        u8g_DrawBox(&u8g, 0, 0, WIDTH, HEIGHT);
        pen = 0;
        u8g_SetColorIndex(&u8g, pen);

        for (i = 0; i < 200; i += 4) {
            u8g_DrawFrame(&u8g, i, i, 200 - i, 200 - i);
        }

    } while (u8g_NextPage(&u8g));
}

void display_frames3()
{
    int i, pen;
    u8g_FirstPage(&u8g);

    do {
        u8g_SetColorIndex(&u8g, 1);
        u8g_DrawBox(&u8g, 0, 0, WIDTH, HEIGHT);
        pen = 0;
        u8g_SetColorIndex(&u8g, pen);

        for (i = 10; i < 90; i += 10) {
            u8g_DrawRFrame(&u8g, i, i, 200 - 2*i, 200 - 2*i, 10);
        }

    } while (u8g_NextPage(&u8g));
}


void display_init()
{
    u8g_Init(&u8g, &xGDEP015OC1u8gDevice);
}
