/* network_game_add_player @0x83782D10 — add a player to a network game if there is room, the player is valid,
 * and no existing slot already holds the same (machine, controller) pair. The player is placed in its own
 * preferred list index when set, otherwise the first free slot. Returns 1 on success.
 *
 * Deviation: the duplicate scan is a 4-way-unrolled loop in the binary; reproduced as a plain 32-slot search
 * (equivalent). */

#include <stdint.h>
#include <string.h>
#include "headers/network_constants.h"
#include "headers/network_game_data.h"
#include "headers/network_player.h"

extern uint8_t network_player_is_valid(network_player *player);

uint8_t network_game_add_player(network_game_data *game, network_player *player)
{
    if ( game->player_count >= game->maximum_players
      || (unsigned int)player->machine_index >= MAXIMUM_NETWORK_MACHINE_COUNT
      || (unsigned int)player->controller_index > MAXIMUM_NUMBER_OF_LOCAL_PLAYERS - 1 )
        return 0;

    /* reject if an existing slot already holds this (machine, controller) pair */
    int existing_match = NETWORK_GAME_MAXIMUM_PLAYER_COUNT;
    for ( int i = 0; i < NETWORK_GAME_MAXIMUM_PLAYER_COUNT; ++i )
    {
        if ( game->players[i].machine_index == player->machine_index
          && game->players[i].controller_index == player->controller_index )
        {
            existing_match = i;
            break;
        }
    }
    if ( existing_match != NETWORK_GAME_MAXIMUM_PLAYER_COUNT || !network_player_is_valid(player) )
        return 0;

    /* first free slot (player_list_index == 0xFF), unless the player has its own preferred index */
    int slot = -1;
    for ( int i = 0; i < NETWORK_GAME_MAXIMUM_PLAYER_COUNT; ++i )
    {
        if ( (uint8_t)game->players[i].player_list_index == 255 )
        {
            slot = i;
            break;
        }
    }
    int preferred = player->player_list_index;
    if ( preferred != -1 && slot != preferred )
        slot = player->player_list_index;
    if ( slot == -1 )
        return 0;

    player->player_list_index = slot;
    memcpy(&game->players[slot], player, sizeof(network_player));
    ++game->player_count;
    return 1;
}
