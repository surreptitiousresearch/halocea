#pragma once
/* icon_hud_element_definition — one entry of hud_globals->messaging.button_icons: names an input
 * icon's sprite sequence, on-screen size/color, and animation. Layout from the database (16 bytes). */

#include <stdint.h>
#include "point2d.h"

typedef struct icon_hud_element_definition
{
    int16_t          sequence_index; /* 0x00 */
    int16_t          width_offset;   /* 0x02 */
    point2d          offset;         /* 0x04 */
    unsigned int     color;          /* 0x08 */
    char             frame_rate;     /* 0x0C */
    uint8_t  flags;          /* 0x0D */
    int16_t          text_index;     /* 0x0E */
} icon_hud_element_definition;       /* 0x10 (16 bytes) */
