/*
    check font
*/


#include "u8g.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>

#include "design/screens_bits.h"

#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))
#define HEIGHT (200)
#define WIDTH (200)

enum screen_index { SCREEN1 = 0, SCREEN2, SCREEN3, SCREEN4, SCREEN5, SCREEN6 };


int u8g_sdl_get_key(void);

/* generated with coordinate_calculator.ods */
static const struct coord {
    int x;
    int y;
} dots[10] = {
    { 100, 40 },
    { 135, 51 },
    { 157, 81 },
    { 157, 119 },
    { 135, 149 },
    { 100, 160 },
    { 65, 149 },
    { 43, 119 },
    { 43, 81 },
    { 65, 51 },
};

static void draw_msgdots(u8g_t *u8g, int num)
{
    unsigned int i;
    const int disc_radius = 10;

    for (i = 0; i < (num % (ARRAY_SIZE(dots) + 1)); i++) {
        u8g_DrawDisc(u8g, dots[i].x, dots[i].y, disc_radius, U8G_DRAW_ALL);
    }
}

int main(int argc, char ** argv)
{
    u8g_t u8g;
    int num_msgs = 0;
    int key = 0;
    enum screen_index si = SCREEN1;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Unable to initialize SDL:  %s\n", SDL_GetError());
        exit(1);
    }

    u8g_Init(&u8g, &u8g_dev_sdl_1bit);

    do {
        u8g_FirstPage(&u8g);
        do
        {
            if (key == 'l') {
                u8g_SetRot90(&u8g);
            } else if (key == 'r') {
                u8g_SetRot270(&u8g);
            } else if (key == 'u') {
                u8g_SetRot180(&u8g);
            } else if (key == 'd') {
                u8g_UndoRotation(&u8g);
            }

            u8g_SetColorIndex(&u8g, 1);
            u8g_DrawBox(&u8g, 0, 0, WIDTH, HEIGHT);
            u8g_SetColorIndex(&u8g, 0);
            u8g_DrawXBM(&u8g, screens[si].x, screens[si].y, screens[si].w, screens[si].h, screens[si].bits);
            draw_msgdots(&u8g, num_msgs);

        } while( u8g_NextPage(&u8g) );

        while( (key = u8g_sdl_get_key()) < 0 );
        if (key == 'n')
            si = (si + 1) % NUM_SCREENS;
        if (key == 'm')
            num_msgs = (num_msgs + 1) % (ARRAY_SIZE(dots) + 1);
        printf("key = %d\n", key);
    } while (key != 32);

    return 0;
}
