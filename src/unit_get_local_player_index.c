/* unit_get_local_player_index @0x836DEE90 — the local-player slot for a unit. The unit's player index lives at
 * unit data +0x218; from that the player datum (512-byte stride) holds its local-player index at +2. Returns -1
 * when the unit is not player-controlled.
 *
 * Attested return int16_t: callee loads the field with lhz (no normalization) and callers extsh r3
 * (biped_bumped_object @0x837AE134) — declared 16-bit signed, caller-side sign extension. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/blam_data_globals.h"


int16_t unit_get_local_player_index(int unit_index)
{
    int player_index = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum)->unit.player_index;
    if ( player_index == -1 )
        return -1;
    return DATA_ARRAY_ELEMENT(player_data, player_datum, player_index)->local_player_index;
}
