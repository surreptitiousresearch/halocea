/* player_ball_count @0x83816340 — count how many oddball spawn slots this player currently owns. */

#include "headers/game_variant.h"
#include "headers/oddball_globals.h"

extern game_variant *game_engine_get_variant(void);

int player_ball_count(int player_index)
{
    int count = 0;
    game_variant *variant = game_engine_get_variant();

    for ( int i = 0; i < variant->game_engine_variant.oddball.ball_spawn_count; ++i )
    {
        if ( oddball_globals.current_ball_owner[i] == player_index )
            ++count;
    }
    return count;
}
