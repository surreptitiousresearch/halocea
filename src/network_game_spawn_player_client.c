/* network_game_spawn_player_client @0x837828C4 — client-side player spawn: creates the local player_datum
 * slot for a network_player (using its real controller index if it belongs to this local machine, else -1),
 * and records the resulting player_list_index back onto the network_player. */

#include <stdint.h>
#include "headers/network_player.h"

extern uint8_t network_game_player_is_local(network_player *player);
extern int player_new_client(int machine_index, int player_index, int16_t local_player_index, network_player *network_player_data);

uint8_t network_game_spawn_player_client(network_player *player, int player_index)
{
    int16_t controller_index = network_game_player_is_local(player) ? player->controller_index : -1;

    player_new_client(player->machine_index, player_index, controller_index, player);

    if ( player_index == -1 )
        return 0;

    player->player_list_index = player_index;
    return 1;
}
