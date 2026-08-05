/* players_apply_server_position_to_specific_client @0x836AB510 — on a client (game_connection 1) for a
 * remote player (no local player index), apply the latest server position to that player's unit if the
 * unit just completed a client update. Vehicle drivers use the vehicle update path (when the new scheme is
 * enabled), everyone else the position path; the just-completed flag is then cleared. */

#include <stdint.h>
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

void players_apply_server_position_to_specific_client(player_datum *player)
{
    if ( game_connection() == _game_connection_network_client && (uint16_t)player->local_player_index == 0xFFFF )
    {
        int unit_index = player->unit_index;
        if ( unit_index != -1 )
        {
            unit_datum *unit = object_try_and_get_and_verify_type(unit_index, object_mask_unit);
            if ( unit )
            {
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
}
