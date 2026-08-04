#pragma once
/* weapon_hud_crosshairs_element — one crosshair entry of a weapon_hud_interface tag's crosshairs block
 * (hud.c). DB-verified layout (types_members): the bitmap + overlay-items pair lives in the nested
 * weapon_hud_crosshair_definition `crosshairs` sub-block at 0x24. */

#include "weapon_hud_crosshair_definition.h"

typedef struct weapon_hud_crosshairs_element
{
    __int16 crosshair_type;                        /* 0x00 — 0..18, selects the crosshair_hud_state.states slot */
    __int16 runtime_flags;                         /* 0x02 */
    __int16 use_on_map_type;                       /* 0x04 — bit index into the game-mode mask (0=in-game, 1=full screen, 2=split screen) */
    __int16 pad;                                   /* 0x06 */
    int     unused[7];                             /* 0x08 */
    weapon_hud_crosshair_definition crosshairs;    /* 0x24 — bitmap @0x24, items @0x34 */
    int     unused2[10];                           /* 0x40 */
} weapon_hud_crosshairs_element;                   /* 0x68 = 104 bytes */
