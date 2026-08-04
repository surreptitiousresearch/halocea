/* ai_scripting_allegiance_broken @0x83770C70 — script query: returns true when the allegiance between two teams
 * is currently "broken", i.e. they are still nominally allied (game_team_is_ally) yet are presently treating each
 * other as enemies (game_team_is_enemy). Invalid team indices return false. */

#include <stdint.h>

extern uint8_t game_team_is_ally(int16_t our_team, int16_t other_team);
extern uint8_t game_team_is_enemy(int16_t our_team, int16_t other_team);

uint8_t ai_scripting_allegiance_broken(int16_t team1_index, int16_t team2_index)
{
    if ( team1_index == -1 || team2_index == -1 )
        return 0;
    if ( !game_team_is_ally(team1_index, team2_index) )
        return 0;
    if ( !game_team_is_enemy(team1_index, team2_index) )
        return 0;
    return 1;
}
