/* network_game_remove_player @0x83783188 — remove a validated player from a network game's 32-slot player
 * table (32-byte stride starting at game+350: machine_index byte+0, controller_index byte+1). Finds the
 * slot matching player's machine/controller index (only among "in-use" slots — controller_index in [0,1],
 * machine_index in [0,15]), resets it to empty (machine/controller/team/list-index all -1, score word
 * zeroed), and decrements the game's player count. Returns whether a matching slot was found and removed.
 *
 * DEVIATION: the decompiler renders the search loop's base pointer as an uninitialized local; disasm
 * confirms it's simply `game + 350` (the function's own first parameter, register-tracking artifact). */

#include <stdint.h>
#include "headers/network_game_data.h"
#include "headers/network_player.h"

extern uint8_t network_game_player_is_valid(network_player *player, network_game_data *game);

extern void network_game_invalidate_player(network_player *player);
int network_game_remove_player(network_game_data *game, network_player *player)
{
    if ( !network_game_player_is_valid(player, game) )
        return 0;

    int slot_index = 0;

    /* recovered: raw `(char*)game + 350` stride-32 walk -> game->players[slot_index] */
    while ( 1 )
    {
        network_player *record = &game->players[slot_index];
        unsigned __int8 in_use = 1;
        if ( (unsigned char)record->controller_index > 1 || record->machine_index < 0 || record->machine_index >= 16 )
            in_use = 0;

        if ( in_use && record->machine_index == player->machine_index && record->controller_index == player->controller_index )
            break;

        if ( ++slot_index >= 32 )
            return 0;
    }

    /* recovered: raw removed[350..353] / (__int16*)removed+173/174/161 -> game->players[slot_index] members */
    /* DEVIATION: inlined copy of network_game_invalidate_player@0x837827F0 (field set/offsets match exactly, store order re-scheduled by compiler); collapsed to a call */
    network_game_invalidate_player(&game->players[slot_index]);

    --game->player_count;
    return 1;
}
