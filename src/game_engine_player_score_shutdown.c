#include "headers/blam_data_globals.h"
/* game_engine_player_score_shutdown @0x8374A498 — clears the per-player score baseline table if it was
 * initialized. Also inlined directly into game_engine_dispose.
 *
 * DEVIATION: the decompiler surfaced memset's r3 as a `void *` return. r3 at blr is only ever the callee's,
 * and the (single, inlined) call site does not consume it — attested void. */

extern void *memset(void *, int, unsigned int);

void game_engine_player_score_shutdown(void)
{
    if (g_player_score_is_initialized == 1)
    {
        memset(g_player_score_baselines, 0, sizeof(g_player_score_baselines));
        g_player_score_is_initialized = 0;
    }
}
