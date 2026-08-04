#pragma once
/* hud_timer_definition — HUD globals sub-block for the mode timer's color scheme (DB
 * types_members-confirmed, 104 bytes). */

#include "hud_color_definition.h"

typedef struct hud_timer_definition
{
    hud_color_definition color;         /* 0x00 */
    hud_color_definition time_up_color; /* 0x20 */
    int                  unused[10];    /* 0x40 */
} hud_timer_definition; /* 104 bytes */
