/* player_update_server_remote_player_position_update_to_network @0x83757660 — server-side: builds and
 * (notionally) broadcasts a remote-player position or vehicle update for one player, then advances
 * that player's remote-update sequence number (mod 8). When the new vehicle-update scheme is active,
 * a player riding (rather than driving) a vehicle uses the vehicle-update encoder, and a non-driving
 * passenger is skipped.
 *
 * DEVIATION: after a successful build the function calls global_network_game_server_get() (return
 * discarded) and then walks the player data array to exhaustion with an empty loop body — faithful to
 * the binary; no message is sent here. */

#include <stdint.h>
#include "headers/player_datum.h"
#include "headers/object_datum.h"
#include "headers/data_array.h"
#include "headers/data_iterator.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"


#include "headers/network_game_server.h"
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern uint8_t player_is_vehicle_driver(int player_index);
extern int build_remote_player_position_update(player_datum *source_player, int player_index, uint8_t *is_reliable_send);
extern int build_remote_player_vehicle_update(player_datum *source_player, int player_index, uint8_t *is_reliable_send);
extern network_game_server *global_network_game_server_get(void);

void player_update_server_remote_player_position_update_to_network(int player_index)
{
    uint8_t update_body[16];
    update_body[0] = 0;

    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);

    int encoded_size;
    if (use_new_vehicle_update_scheme)
    {
        object_datum *unit = object_try_and_get_and_verify_type(player->unit_index, object_mask_unit);
        if (!unit)
            return;
        if (unit->object.parent_object_index == -1)   /* +284: not in a vehicle */
        {
            encoded_size = build_remote_player_position_update(player, player_index, update_body);
        }
        else
        {
            if (player_is_vehicle_driver(player_index) != 1)
                return;
            encoded_size = build_remote_player_vehicle_update(player, player_index, update_body);
        }
    }
    else
    {
        encoded_size = build_remote_player_position_update(player, player_index, update_body);
    }

    if (encoded_size > 0)
    {
        global_network_game_server_get();

        data_iterator iterator;
        data_iterator_new(&iterator, player_data);
        while (data_iterator_next(&iterator))
            ;

        player->___u26.server_update_data.next_remote_player_update_sequence_number =
            (player->___u26.server_update_data.next_remote_player_update_sequence_number + 1) % 8;
    }
}
