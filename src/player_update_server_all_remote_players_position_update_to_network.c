/* player_update_server_all_remote_players_position_update_to_network @0x83757768 — (server) build and
 * queue each player's position/vehicle replication update: for each player datum, if the legacy vehicle
 * update scheme is in effect, or the player has no controlled unit, always build a position update;
 * otherwise (new scheme, unit valid) only vehicle drivers get a vehicle update built, everyone else is
 * skipped this tick. On a successful build (result > 0), touches the network game server singleton and
 * fully drains a fresh player_data iterator (side effect only — result discarded, matches shipped code),
 * then advances the player's own remote-update sequence number mod 8. Returns the last player datum
 * iterated (matches the decompiled return type/behavior). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/data_iterator.h"
#include "headers/player_datum.h"
#include "headers/object_datum.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"


#include "headers/network_game_server.h"
extern void data_iterator_new(data_iterator *iterator, data_array *data);
extern void *data_iterator_next(data_iterator *iterator);
extern int build_remote_player_position_update(player_datum *source_player, int player_index,
        uint8_t *is_reliable_send);
extern int build_remote_player_vehicle_update(player_datum *source_player, int player_index, uint8_t *is_reliable_send);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern uint8_t player_is_vehicle_driver(int player_index);
extern network_game_server *global_network_game_server_get(void);

void *player_update_server_all_remote_players_position_update_to_network(void)
{
    data_iterator iterator;
    data_iterator_new(&iterator, player_data);

    void *result;
    while ( (result = data_iterator_next(&iterator)) != nullptr )
    {
        uint8_t is_reliable_send = 0;
        int player_index = iterator.index;
        player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, iterator.index);

        int update_size = 0;
        char updated = 0;

        if ( !use_new_vehicle_update_scheme )
        {
            update_size = build_remote_player_position_update(player, player_index, &is_reliable_send);
            updated = 1;
        }
        else
        {
            object_datum *unit = object_try_and_get_and_verify_type(player->unit_index, object_mask_unit);
            if ( unit )
            {
                if ( unit->object.parent_object_index == -1 )
                {
                    update_size = build_remote_player_position_update(player, player_index, &is_reliable_send);
                    updated = 1;
                }
                else if ( player_is_vehicle_driver(player_index) == 1 )
                {
                    update_size = build_remote_player_vehicle_update(player, player_index, &is_reliable_send);
                    updated = 1;
                }
            }
        }

        if ( updated && update_size > 0 )
        {
            global_network_game_server_get();

            data_iterator drain_iterator;
            data_iterator_new(&drain_iterator, player_data);
            while ( data_iterator_next(&drain_iterator) )
                ;

            player->___u26.server_update_data.next_remote_player_update_sequence_number =
                (player->___u26.server_update_data.next_remote_player_update_sequence_number + 1) % 8;
        }
    }

    return result;
}
