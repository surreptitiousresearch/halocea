/* player_handle_powerup @0x836AC528 — apply a timed powerup to a player. powerup_type is 0 (active
 * camo) or 1 (the other timed powerup); anything else is rejected. Camo additionally requires a living
 * unit that is not already camouflaged (unit.flags 0x10). The per-type timer (powerup_durations[type])
 * is extended by duration_in_ticks; if it was inactive the powerup is turned on, and a non-multiplayer
 * camo activation marks the unit's camo-changed bit (0x20). On a server (connection 2) with a
 * locally-owned unit the powerup is replicated to the network. Returns 1 on success. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/player_datum.h"
#include "headers/game_connection.h"
#include "headers/unit_flags.h"
#include "headers/object_type.h"
#include "headers/player_powerup.h"
#include "headers/blam_data_globals.h"


extern void *datum_try_and_get(const data_array *data, int index);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern uint8_t game_engine_running(void);
extern void player_powerup_on(int player_index, int16_t powerup_type);
extern int16_t game_connection(void);
extern void player_handle_powerup_to_network(int player_index, int16_t powerup_type, int16_t duration_in_ticks);

uint8_t player_handle_powerup(int player_index, int16_t powerup_type, int16_t duration_in_ticks)
{
    player_datum *player = datum_try_and_get(player_data, player_index);
    if (!player)
        return 0;
    if ((unsigned int)powerup_type > 1)
        return 0;

    if (powerup_type == _player_powerup_active_camouflage)
    {
        unit_datum *camo_unit = object_try_and_get_and_verify_type(player->unit_index, object_mask_unit);
        if (!camo_unit || (camo_unit->unit.flags & (1u << _unit_active_camouflaged_bit)) != 0)
            return 0;
    }

    if (player->powerup_durations[powerup_type])
    {
        if (!game_engine_running() && powerup_type == _player_powerup_active_camouflage)
        {
            unit_datum *unit = (unit_datum *)
                DATA_ARRAY_ELEMENT(object_header_data, object_header_datum,
                                   DATA_ARRAY_ELEMENT(player_data, player_datum, player_index)->unit_index)->datum;
            unit->unit.flags |= (1u << _unit_super_camouflaged_bit);
        }
    }
    else
    {
        player_powerup_on(player_index, powerup_type);
    }

    player->powerup_durations[powerup_type] += duration_in_ticks;

    if (game_connection() == _game_connection_network_server
        && !DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, player->unit_index)->datum
                ->object.datum_role)
        player_handle_powerup_to_network(player_index, powerup_type, duration_in_ticks);

    return 1;
}
