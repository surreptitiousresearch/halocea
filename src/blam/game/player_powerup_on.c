/* player_powerup_on @0x836AA480 — turn on a player's powerup state. Only active camo (powerup_type 0)
 * has any on-state to set here: it marks the player's unit invisible (unit.flags 0x10) and clears
 * the camo-regrowth word. Any other powerup type is a no-op. The unit is resolved from the player
 * datum's unit index. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/player_datum.h"
#include "headers/unit_flags.h"
#include "headers/player_powerup.h"
#include "headers/blam_data_globals.h"


void player_powerup_on(int player_index, int16_t powerup_type)
{
    unit_datum *unit = (unit_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum,
                           DATA_ARRAY_ELEMENT(player_data, player_datum, player_index)->unit_index)->datum;

    if (powerup_type == _player_powerup_active_camouflage)
    {
        unit->unit.cause_for_camo_regrowth = 0;
        unit->unit.flags |= (1u << _unit_active_camouflaged_bit);
    }
}
