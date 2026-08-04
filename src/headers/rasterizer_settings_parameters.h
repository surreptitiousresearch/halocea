#pragma once

typedef struct rasterizer_settings_parameters {
    unsigned int  screen_width;    /* 0x00 */
    unsigned int  screen_height;   /* 0x04 */
    unsigned int  refresh_rate;    /* 0x08 */
    unsigned char vsync;           /* 0x0C */
} rasterizer_settings_parameters;  /* sizeof = 0x10 */
