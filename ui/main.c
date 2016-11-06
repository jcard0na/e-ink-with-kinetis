/*
    check font
*/

#include "u8g.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define HEIGHT (200)
#define WIDTH (200)

int u8g_sdl_get_key(void);

#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]));

static char initial[] = "Everytime I find";
static char second[] = "THE MEANING OF LIFE";
static char numbers1[] = "1234567890";
static char numbers2[] = "1234567890";

int main(void)
{
    u8g_t u8g;
    int w;
    int key;

    u8g_Init(&u8g, &u8g_dev_sdl_1bit);

    do {
        u8g_FirstPage(&u8g);
        do
        {
            u8g_SetColorIndex(&u8g, 1);
            u8g_DrawDisc(&u8g, WIDTH/2, HEIGHT/2, WIDTH/2, U8G_DRAW_ALL);

            u8g_SetColorIndex(&u8g, 0);

            u8g_SetFont(&u8g, u8g_unifont_upper);
            u8g_SetFontPosCenter(&u8g);
            w = u8g_GetStrWidth(&u8g, initial);
            u8g_DrawStr(&u8g, WIDTH/2 - w/2, HEIGHT/2 - 20, initial);

            u8g_SetFont(&u8g, u8g_font_profont22r);
            u8g_SetFontPosCenter(&u8g);
            w = u8g_GetStrWidth(&u8g, initial);
            u8g_DrawStr(&u8g, WIDTH/2 - w/2, HEIGHT/2, initial);

            u8g_SetFont(&u8g, u8g_font_freedoomr10r);
            u8g_SetFontPosCenter(&u8g);
            w = u8g_GetStrWidth(&u8g, second);
            u8g_DrawStr(&u8g, WIDTH/2 - w/2, HEIGHT/2 + 20, second);

            u8g_SetFont(&u8g, u8g_font_profont22r);
            u8g_SetFontPosCenter(&u8g);
            w = u8g_GetStrWidth(&u8g, numbers1);
            u8g_DrawStr(&u8g, WIDTH/2 - w/2, HEIGHT/2 + 40, numbers1);

            u8g_SetFont(&u8g, u8g_font_freedoomr10r);
            u8g_SetFontPosCenter(&u8g);
            w = u8g_GetStrWidth(&u8g, numbers2);
            u8g_DrawStr(&u8g, WIDTH/2 - w/2, HEIGHT/2 + 60, numbers2);

        } while( u8g_NextPage(&u8g) );

        while( (key = u8g_sdl_get_key()) < 0 );
        printf("key = %d\n", key);
    } while (key != 32);

    return 0;
}
