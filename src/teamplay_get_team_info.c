/* teamplay_get_team_info @0x83802AE8 — count the valid network players on teams 0/1 (skipping
 * skip_player_index) and fetch both team scores. Zeroes all outputs when no team game is running.
 *
 * Faithful shipped quirk (bug class 12, disasm-verified): the compiled loop walks the 32 network-player
 * records at ABSOLUTE addresses (record k at 0x142 + 32k, team byte at 0x160 + 32k, player index byte at
 * 0x161 + 32k) — the base returned by global_network_game_server_get (called and discarded) was never
 * added. Reproduced verbatim, including the discarded call. */

#include <stdint.h>
#include "headers/network_player.h"
#include "headers/multiplayer_team.h"
#include "headers/network_game_server.h"

extern uint8_t game_engine_running(void);
extern uint8_t game_engine_has_teams(void);
extern network_game_server *global_network_game_server_get(void);
extern uint8_t network_player_is_valid(network_player *player);
extern int game_engine_get_team_score(int team_index);

void teamplay_get_team_info(int *players, int *scores, int skip_player_index)
{
    scores[0] = 0;
    scores[1] = 0;
    players[1] = 0;
    players[0] = 0;

    if ( !game_engine_running() || !game_engine_has_teams() )
        return;

    global_network_game_server_get();   /* result discarded, as compiled */
    /* recovered: absolute-address walk (record@0x160 = base+0x1E) -> typed network_player at base 0x142 */
    for ( int address = 0x142; address < 0x542; address += 32 )
    {
        network_player *player = (network_player *)address;
        if ( network_player_is_valid(player)
          && player->player_list_index != skip_player_index
          && (unsigned int)player->team_index <= _multiplayer_team_blue )
        {
            ++players[player->team_index];
        }
    }

    scores[0] = game_engine_get_team_score(0);
    scores[1] = game_engine_get_team_score(1);
}
