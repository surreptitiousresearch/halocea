#pragma once
/* crosshair_hud_state — per-local-player runtime state for the 19 weapon-HUD crosshair overlays. One 4-byte
 * crosshair_state slot per overlay (a timestamp, a countdown, a zoom sub-value, or a float frame_index
 * depending on the state index) plus a bitmask of which overlays are currently rendered. 80 bytes; matches
 * the 0x50 memset in hud_update_weapon_local_player and the crosshair_state[2] stride in
 * weapon_hud_globals_definition. */

#include "crosshair_state.h"

typedef struct crosshair_hud_state
{
    crosshair_state states[19];   /* 0x00 */
    unsigned int    render_flags; /* 0x4C */
} crosshair_hud_state;            /* 0x50 = 80 bytes */
