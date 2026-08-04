#pragma once
/* color_table_color — one named ARGB entry in an interface color-table tag (48 bytes). Full DB layout. */
#include "real_argb_color.h"

typedef struct color_table_color
{
    char            name[32];      /* 0x00 */
    real_argb_color real_color;    /* 0x20 */
} color_table_color;               /* 48 bytes */
