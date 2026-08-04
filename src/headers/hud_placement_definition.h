#pragma once
/* hud_placement_definition — anchored screen offset + scale shared by HUD elements (hud.c). Layout
 * from the database (36 bytes). */

#include "point2d.h"
#include "real_vector2d.h"

typedef struct hud_placement_definition
{
    point2d       offset;                    /* 0x00 */
    real_vector2d scale;                     /* 0x04 */
    __int16       multiplayer_scaling_flags; /* 0x0C */
    __int16       pad;                       /* 0x0E */
    int           unused0[5];                /* 0x10 */
} hud_placement_definition;                  /* 0x24 */
