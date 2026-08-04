/* network_game_update_player @0x83783120 — applies a network_player update to its slot in the game's
 * player table, after verifying the update is for a still-valid, still-same (machine, controller) slot.
 *
 * DEVIATION: the decompiler computed the destination as `32 * player->player_list_index + <lost base>`
 * with no visible base assignment, and read the verification fields via raw offsets 350/351 off that
 * incomplete pointer. Disasm (r11 = game + 32*player_list_index, r3 = r11 + 0x142) plus
 * network_game_data.h's `players[32]` at offset 0x142 confirm r11+350/r11+351 are simply
 * game->players[index].machine_index/.controller_index (0x142 + 28/29) reached before the +0x142 struct
 * base was folded in — i.e. this is exactly `&game->players[player->player_list_index]`. */

#include <stdint.h>
#include <string.h>
#include "headers/network_game_data.h"

extern uint8_t network_game_player_is_valid(network_player *player, network_game_data *game);

uint8_t network_game_update_player(network_game_data *game, network_player *player)
{
    if ( !network_game_player_is_valid(player, game) )
        return 0;

    network_player *slot = &game->players[player->player_list_index];
    if ( slot->controller_index != player->controller_index || slot->machine_index != player->machine_index )
        return 0;

    memcpy(slot, player, sizeof(*slot));
    return 1;
}
