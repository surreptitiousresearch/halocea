/* player_control_get_zoom_level @0x836DEF98 — current desired zoom level for a local player, or -1 if the
 * player index is invalid.
 *
 * Attested return int16_t: callee loads the field with lhz (no normalization) and callers extsh r3
 * (first_person_weapon_render_update @0x8369F898 et al.) — declared 16-bit signed, caller-side extension. */

#include <stdint.h>
#include "headers/player_control_globals.h"

int16_t player_control_get_zoom_level(int16_t local_player_index)
{
    if ( local_player_index == -1 )
        return -1;
    return player_control_globals->players[local_player_index].desired_zoom_level;
}
