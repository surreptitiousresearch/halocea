#pragma once
#include <stdint.h>
/* hud_absolute_placement_definition — screen-corner anchor for a directly-placed HUD element
 * (hud.c). Layout from the database (36 bytes). */

typedef struct hud_absolute_placement_definition
{
    int16_t corner;     /* 0x00 */
    int16_t pad;        /* 0x02 */
    int     unused[8];  /* 0x04 */
} hud_absolute_placement_definition;  /* 0x24 */
