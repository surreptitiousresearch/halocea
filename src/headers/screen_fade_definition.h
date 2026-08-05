#pragma once
/* screen_fade_definition — the active full-screen color fade driven by the player-effect system.
 * Layout from the database (20 bytes). */

#include <stdint.h>
#include "real_rgb_color.h"

typedef struct screen_fade_definition
{
    real_rgb_color color;        /* 0x00 */
    int            start_time;   /* 0x0C — game tick the fade began */
    int16_t        ticks;        /* 0x10 — fade duration in ticks */
    uint8_t fading_out;  /* 0x12 — nonzero while fading out */
    uint8_t _pad13;      /* 0x13 — tail padding to 4-byte alignment */
} screen_fade_definition;        /* 20 bytes */
