#pragma once
/* render_screen_flash — the per-frame screen-flash/fade overlay handed to the renderer (0x18 bytes). */

#include <stdint.h>
#include "real_argb_color.h"

typedef struct render_screen_flash
{
    int16_t         type;       /* 0x00 */
    unsigned char _pad0[2]; /* db-verified padding */
    float           intensity;  /* 0x04 */
    real_argb_color color;      /* 0x08 */
} render_screen_flash;          /* 0x18 */
