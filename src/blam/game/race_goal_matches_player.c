/* race_goal_matches_player @0x8382AED0 — true when a player's team has completed the lap for goal_index
 * and can touch that team (race "flag" goal check). */

#include <stdint.h>
#include "headers/race_globals.h"

extern uint8_t can_touch_team(int player_index, int team_index);

uint8_t race_goal_matches_player(int player_index, int goal_index)
{
    if ((1 << goal_index) & race_globals.lap_completed_value)
        return can_touch_team(player_index, goal_index);
    return 0;
}
