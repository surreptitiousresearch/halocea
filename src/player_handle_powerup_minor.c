/* player_handle_powerup_minor @0x836AC6A4 — grant a player a "minor" (timed) powerup, extending its timer
 * to at least minor_duration. The per-powerup timer is player->powerup_durations[powerup_type]. Powerup
 * type 0 (active camouflage), when first applied, sets the camo flag on the player's unit (unit.flags
 * 0x10) and clears its camo-regrowth word. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/player_datum.h"
#include "headers/unit_flags.h"
#include "headers/player_powerup.h"
#include "headers/blam_data_globals.h"


void player_handle_powerup_minor(int player_index, int16_t powerup_type, int16_t minor_duration)
{
    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
    if ( !player->powerup_durations[powerup_type] )
    {
        unit_datum *unit = (unit_datum *)
            DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, player->unit_index)->datum;
        if ( powerup_type == _player_powerup_active_camouflage )
        {
            unit->unit.flags |= (1u << _unit_active_camouflaged_bit);
            unit->unit.cause_for_camo_regrowth = 0;
        }
    }
    int16_t current = player->powerup_durations[powerup_type];
    player->powerup_durations[powerup_type] = current <= minor_duration ? minor_duration : current;
}
