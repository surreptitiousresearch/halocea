/* player_control_unzoom @0x836DEF20 — cancel any zoom for the local player controlling `unit_index` by
 * setting that player's desired zoom level to -1. Resolves unit -> owning player datum (object data +536) ->
 * local-player index (player datum +2, 512-byte stride); a no-op when the unit has no player or no local
 * player slot. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/player_control_globals.h"
#include "headers/blam_data_globals.h"


void player_control_unzoom(int unit_index)
{
    int player_index = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum)->unit.player_index;

    int16_t local_player_index;
    if ( player_index == -1 )
        local_player_index = -1;
    else
        local_player_index = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index)->local_player_index;

    if ( local_player_index != -1 )
        player_control_globals->players[local_player_index].desired_zoom_level = -1;
}
