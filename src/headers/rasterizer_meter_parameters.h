#pragma once
/* rasterizer_meter_parameters — the color/gradient parameters handed to the meter rasterizer by
 * hud_draw_meter (28 bytes, DB layout). gradient_min/max_color are packed ARGB endpoints, background_color
 * fills the un-drawn remainder, flash_color drives the flash animation. */

typedef struct rasterizer_meter_parameters
{
    unsigned int    gradient_min_color;      /* 0x00 */
    unsigned int    gradient_max_color;      /* 0x04 */
    unsigned int    background_color;        /* 0x08 */
    unsigned int    flash_color;             /* 0x0C */
    unsigned __int8  flash_color_is_negative; /* 0x10 */
    unsigned __int8  tint_mode_2;            /* 0x11 */
    unsigned char   pad12[2];                /* 0x12 */
    unsigned int    tint_color;              /* 0x14 */
    float           gradient;                /* 0x18 */
} rasterizer_meter_parameters;               /* 0x1C = 28 bytes */
