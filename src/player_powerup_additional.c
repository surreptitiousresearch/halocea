/* player_powerup_additional @0x836AA4E0 — apply the extra on-state for a re-triggered powerup.
 * Only active camo (powerup_type 0) does anything: it marks the player's unit super-camouflaged
 * (unit.flags 0x20). Any other powerup type is a no-op. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/player_datum.h"
#include "headers/unit_flags.h"
#include "headers/player_powerup.h"
#include "headers/blam_data_globals.h"


void player_powerup_additional(int player_index, int16_t powerup_type)
{
    if (powerup_type == _player_powerup_active_camouflage)
    {
        unit_datum *unit = (unit_datum *)
            DATA_ARRAY_ELEMENT(object_header_data, object_header_datum,
                               DATA_ARRAY_ELEMENT(player_data, player_datum, player_index)->unit_index)->datum;
        unit->unit.flags |= (1u << _unit_super_camouflaged_bit);
    }
}
