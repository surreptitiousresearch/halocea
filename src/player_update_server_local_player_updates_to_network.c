/* player_update_server_local_player_updates_to_network @0x83757558 — server-side: for every remote
 * (non-local) player that has a unit, build a position-or-vehicle update message and send it to that
 * player's machine over the network game server.
 *
 * Deviation: the decompiler's player_datum* return (the last iterator result) is discarded — void. */

#include <stdint.h>
#include "headers/data_iterator.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"


#include "headers/data_array.h"
#include "headers/network_game_server.h"
extern int16_t          network_message_type_message_delta;

extern void data_iterator_new(data_iterator *iterator, data_array *data);
extern void *data_iterator_next(data_iterator *iterator);
extern char *initialize_message_buffer(char *buffer, int buffer_size, int *maximum_content_size);
extern uint8_t player_is_vehicle_driver(int player_index);
extern int build_local_player_vehicle_update(player_datum *source_player, int player_index, uint8_t *is_reliable_send);
extern int build_local_player_position_update(player_datum *source_player, int player_index, uint8_t *is_reliable_send);
extern network_game_server *global_network_game_server_get(void);
extern uint8_t network_game_server_send_message_to_machine(struct network_game_server *server, int machine_index, int type, void *message, int size_in_bits, uint8_t reliable, uint8_t immediate, uint8_t including_local_client, int priority);

void player_update_server_local_player_updates_to_network(void)
{
    data_iterator iter;
    player_datum *player;
    uint8_t update_scratch[16];
    int message_out[4];
    uint8_t message_buffer[48];

    data_iterator_new(&iter, player_data);
    for ( player = data_iterator_next(&iter); player;
          player = data_iterator_next(&iter) )
    {
        if ( (uint16_t)player->local_player_index == 0xFFFF && player->unit_index != -1 )
        {
            int length;
            initialize_message_buffer(message_buffer, 4095, message_out);
            if ( player_is_vehicle_driver(iter.index) && use_new_vehicle_update_scheme )
                length = build_local_player_vehicle_update(player, iter.index, update_scratch);
            else
                length = build_local_player_position_update(player, iter.index, update_scratch);
            if ( length > 0 )
            {
                void *server = global_network_game_server_get();
                network_game_server_send_message_to_machine(
                    server,
                    player->network_player_data.machine_index,
                    network_message_type_message_delta,
                    g_message_encode_buffer,
                    length,
                    0, 0, 0, message_out[0]);
            }
        }
    }
}
