/* players_apply_server_position_to_other_clients @0x836ACE48 — on a network client, push the
 * server's authoritative position for every remote (non-local) player onto that player's unit.
 *
 * For each player that is not local (local_player_index == 0xFFFF) and has an attached unit, if the
 * unit just completed a client update, apply either the vehicle update (when the player is driving a
 * vehicle and the new vehicle-update scheme is enabled) or the plain position update, then clear the
 * did_just_complete_client_update flag.
 *
 * Deviation: the decompiler's player_datum* return (the last iterator result) is discarded — void. */

#include <stdint.h>
#include "headers/data_iterator.h"
#include "headers/player_datum.h"
#include "headers/unit_datum.h"
#include "headers/game_connection.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"


extern int16_t game_connection(void);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern uint8_t player_is_vehicle_driver(int player_index);
extern void apply_remote_player_vehicle_update(player_datum *player, unit_datum *unit);
extern void apply_remote_player_position_update(player_datum *player, unit_datum *unit);

void players_apply_server_position_to_other_clients(void)
{
    data_iterator iter;
    player_datum *player;

    if ( game_connection() != _game_connection_network_client )
        return;

    data_iterator_new(&iter, player_data);
    for ( player = data_iterator_next(&iter); player;
          player = data_iterator_next(&iter) )
    {
        if ( game_connection() == _game_connection_network_client && (unsigned __int16)player->local_player_index == 0xFFFF )
        {
            int unit_index = player->unit_index;
            unit_datum *unit;
            if ( unit_index == -1 )
                continue;
            unit = object_try_and_get_and_verify_type(unit_index, object_mask_unit);
            if ( !unit )
                continue;
            if ( unit->unit.did_just_complete_client_update == 1 )
            {
                if ( player_is_vehicle_driver(unit->unit.player_index) && use_new_vehicle_update_scheme )
                    apply_remote_player_vehicle_update(player, unit);
                else
                    apply_remote_player_position_update(player, unit);
                unit->unit.did_just_complete_client_update = 0;
            }
        }
    }
}
