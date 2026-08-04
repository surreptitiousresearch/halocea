#pragma once
/* unit_hud_reference — one element of a unit_seat's seat_huds tag block (48 bytes per DB
 * types_members). The referenced HUD tag index lives in hud.index (+12). */

#include "tag_reference.h"

typedef struct unit_hud_reference
{
    tag_reference hud;         /* 0x00 */
    int           unused[8];   /* 0x10 */
} unit_hud_reference;          /* 48 bytes */
