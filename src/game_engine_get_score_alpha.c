/* game_engine_get_score_alpha @0x83745F98 — score-display alpha for team 0, only while the game engine mode
 * is game_engine_mode_active; any post-game mode forces alpha to 0. */

#include "headers/game_engine_globals.h"
#include "headers/blam_data_globals.h"


float game_engine_get_score_alpha(void)
{
    if (game_engine_globals.mode)
        return 0.0f;
    return game_engine_globals.score_alpha[0];
}
