/* goal_matches_player @0x8374E0C8 */
#include <stdint.h>
#include "headers/player_datum.h"
#include "headers/game_engine.h"
#include "headers/netgame_goal.h"
#include "headers/game_engine_type.h"
#include "headers/blam_data_globals.h"

/* game_engine: canonical decl from game_engine.h (via blam_data_globals.h) */
extern uint8_t ctf_somebody_has_my_flag_in_vehicle(player_datum *player);

uint8_t goal_matches_player(player_datum *player, int player_index, int goal_index)
{
    if (!game_engine)
        return 0;
    netgame_goal *goal = &global_goal[goal_index];
    if (game_engine->type == game_engine_ctf)
    {
        if (goal->in_use)
        {
            int pi = goal->player_index;
            if (pi == -1 || player_index == pi)
            {
                int ti = goal->team_index;
                if (ti == -1 || player->team_index == ti)
                {
                    int ii = goal->ignore_player_index;
                    if (ii == -1 || player_index != ii)
                        return 1;
                }
            }
            if (player->team_index == goal->team_index || goal->ignore_player_index == -1)
                return 0;
            return ctf_somebody_has_my_flag_in_vehicle(player) == 0 ? 0 : 1;
        }
        return 0;
    }
    else
    {
        unsigned char (*fn)(int, int) = game_engine->goal_matches_player;
        if (fn)
        {
            if (goal->in_use)
                return fn(player_index, goal_index);
            return 0;
        }
        if (!goal->in_use)
            return 0;
        int pi = goal->player_index;
        if (pi != -1 && player_index != pi)
            return 0;
        int ti = goal->team_index;
        if (ti != -1 && player->team_index != ti)
            return 0;
        int ii = goal->ignore_player_index;
        if (ii == -1)
            return 1;
        return (player_index == ii) ? 0 : 1;
    }
}
