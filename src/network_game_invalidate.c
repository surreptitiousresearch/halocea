/* network_game_invalidate @0x83782C74 — reset a network_game_data to an empty 32-slot session: clear it, set
 * each player slot's identity/index fields to the -1 sentinel, set the max player count, and mark whether game
 * objects are already loaded (true only mid client-reconnect).
 *
 * The decompiler walks the player slots with a raw char pointer starting at &_unused_game_engine
 * (stride 32 == sizeof(network_player)); re-expressed as named game->players[i] member access. */

#include <string.h>
#include "headers/network_game_data.h"
#include "headers/network_player.h"
#include "headers/blam_data_globals.h"


extern void network_game_invalidate_player(network_player *player);
void network_game_invalidate(network_game_data *game)
{
    memset(game, 0, sizeof(network_game_data));
    memset(&game->map, 0, sizeof(game->map));
    game->player_count = 0;

    /* DEVIATION: inlined copy of network_game_invalidate_player@0x837827F0, unrolled into this 32-slot reset loop by the compiler; collapsed back to a call */
    for ( int i = 0; i < 32; ++i )
        network_game_invalidate_player(&game->players[i]);

    game->maximum_players = 32;
    game->local_data.game_objects_loaded = g_HACK_client_reconnect_attempt_in_progress ? 1 : 0;
}
