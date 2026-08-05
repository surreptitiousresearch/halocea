#pragma once
/* hud_placement_definition — anchored screen offset + scale shared by HUD elements (hud.c). Layout
 * from the database (36 bytes). */

#include <stdint.h>
#include "point2d.h"
#include "real_vector2d.h"

typedef struct hud_placement_definition
{
    point2d       offset;                    /* 0x00 */
    real_vector2d scale;                     /* 0x04 */
    int16_t       multiplayer_scaling_flags; /* 0x0C */
    int16_t       pad;                       /* 0x0E */
    int           unused0[5];                /* 0x10 */
} hud_placement_definition;                  /* 0x24 */
