#include "headers/blam_data_globals.h"
/* game_engine_player_score_shutdown @0x8374A498 — clears the per-player score baseline table if it was
 * initialized. Also inlined directly into game_engine_dispose. */

extern void *memset(void *, int, unsigned int);

void *game_engine_player_score_shutdown(void)
{
    void *result = 0;
    if (g_player_score_is_initialized == 1)
    {
        result = memset(g_player_score_baselines, 0, sizeof(g_player_score_baselines));
        g_player_score_is_initialized = 0;
    }
    return result;
}
