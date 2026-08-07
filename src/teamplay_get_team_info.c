/* teamplay_get_team_info @0x83802AE8 — count the valid network players on teams 0/1 (skipping
 * skip_player_index) and fetch both team scores. Zeroes all outputs when no team game is running.
 *
 * NOT a base this reconstruction dropped (re-derived 2026-08-07): the loop init at 0x83802B38 encodes as
 * 3BE00160 = `addi r31, r0, 0x160`, rA = r0. 0x142 + 32k IS network_game_data.players[k] (DB
 * types_members) over the NULL that the network_game_server_get_game() stub folds in — mechanism in
 * network_game_server_get_game.c. The discarded getter call is reproduced because the binary makes it. */

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
