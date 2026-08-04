/* multiple_teams_alive @0x8374B568 — with 2+ players in the game, check whether more than one distinct
 * team still has a "still in it" player: a player counts as alive unless they've quit
 * (quit_out_of_game), are the game engine's designated "odd man out", have no unit and have already used
 * up all their lives (statistics.deaths >= universal_variant.lives, when a lives limit is set), or have a
 * unit but it's simply absent (unit_index == -1 without the lives check, which alone marks them counted
 * as still alive/relevant). Returns 1 immediately if 1 or fewer players are in the game, 1 as soon as a
 * second distinct team with an alive player is found, 0 if only one team (or no team) qualifies. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/data_iterator.h"
#include "headers/player_datum.h"
#include "headers/game_variant.h"
#include "headers/blam_data_globals.h"


extern int players_in_game(void);
extern uint8_t game_engine_player_is_odd_man_out(int player_index);

uint8_t multiple_teams_alive(void)
{
    if ( players_in_game() <= 1 )
        return 1;

    int first_alive_team = -1;

    data_iterator iterator;
    data_iterator_new(&iterator, player_data);
    player_datum *player = data_iterator_next(&iterator);

    while ( player )
    {
        unsigned __int8 alive;

        if ( player->quit_out_of_game )
        {
            alive = 0;
        }
        else if ( player->unit_index != -1 )
        {
            alive = 1;
        }
        else if ( game_engine_player_is_odd_man_out(iterator.index) )
        {
            alive = 0;
        }
        else
        {
            unsigned __int8 used_up_lives = 0;
            if ( global_variant.universal_variant.lives > 0 )
            {
                player_datum *player_by_index = DATA_ARRAY_ELEMENT(player_data, player_datum, iterator.index);
                if ( player_by_index->unit_index == -1 )
                    used_up_lives = player_by_index->statistics.deaths >= global_variant.universal_variant.lives;
            }
            alive = !used_up_lives;
        }

        if ( alive && player->team_index != first_alive_team )
        {
            if ( first_alive_team != -1 )
                return 1;
            first_alive_team = player->team_index;
        }

        player = data_iterator_next(&iterator);
    }

    return 0;
}
