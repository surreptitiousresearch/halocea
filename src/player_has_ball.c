/* player_has_ball @0x838163B8 — true if the given player currently owns any spawned oddball. */

#include <stdint.h>
#include "headers/game_variant.h"
#include "headers/oddball_globals.h"

extern game_variant *game_engine_get_variant(void);

uint8_t player_has_ball(int player_index)
{
    int ball_spawn_count = game_engine_get_variant()->game_engine_variant.oddball.ball_spawn_count;

    for ( int i = 0; i < ball_spawn_count; ++i )
    {
        if ( oddball_globals.current_ball_owner[i] == player_index )
            return 1;
    }

    return 0;
}
