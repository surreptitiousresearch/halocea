/* network_game_spawn_player @0x83782818 — create the local player datum for a network player.
 * Local players bind to their controller index; remote players pass -1 (no controller). The new
 * player-list index is stored back on the network_player. Returns 1 on success, 0 if player_new
 * could not allocate a slot. */

#include <stdint.h>
#include "headers/network_player.h"

extern uint8_t network_game_player_is_local(network_player *player);
extern int player_new(int machine_index, int player_index, int16_t local_player_index, network_player *network_player_data);

uint8_t network_game_spawn_player(network_player *player)
{
    int16_t controller_index;
    if (network_game_player_is_local(player))
        controller_index = player->controller_index;
    else
        controller_index = -1;

    int new_player_index = player_new(player->machine_index, player->player_list_index,
                                      controller_index, player);
    if (new_player_index == -1)
        return 0;

    player->player_list_index = new_player_index;
    return 1;
}
