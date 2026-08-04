/* network_game_player_is_valid @0x83783060 — a network_player is valid if its controller_index is 0/1 and its
 * machine_index is a plausible in-range machine (0-15), AND it's actually found among the game's 32 player
 * slots by matching (machine_index, controller_index). */

#include <stdint.h>
#include "headers/network_player.h"
#include "headers/network_game_data.h"

uint8_t network_game_player_is_valid(network_player *player, network_game_data *game)
{
    if ( !player
      || (unsigned int)player->controller_index > 1
      || player->machine_index < 0
      || player->machine_index >= 16 )
    {
        return 0;
    }

    for ( int i = 0; i < 32; i++ )
    {
        network_player *slot = &game->players[i];
        if ( slot->machine_index == player->machine_index && slot->controller_index == player->controller_index )
            return 1;
    }

    return 0;
}
