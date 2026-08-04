/* game_engine_progress_to_postgame_rasterize @0x8374A7A8 — advance the game engine into the
 * postgame-rasterize phase, then arm (or disable) the map-cycle countdown depending on whether the
 * server is force-stopping and on the configured timeout.
 *
 * DEVIATION: the decompiler's __int64 register juggling (HIDWORD/LODWORD of v0) is a soft-float/64-bit
 * artifact; the stored value is simply (float)gMapCycleTimeoutSeconds. */

#include "headers/game_engine_globals.h"
#include "headers/blam_data_globals.h"


void game_engine_progress_to_postgame_rasterize(void)
{
    game_engine_globals.mode = game_engine_mode_postgame_rasterize;
    if ( gForceServerToStop == 1 )
    {
        game_engine_globals.map_cycle_countdown_enabled = 0;
        game_engine_globals.time_until_map_cycle_timeout_expires_seconds = 0.0f;
    }
    else if ( gMapCycleTimeoutSeconds > 0 )
    {
        game_engine_globals.map_cycle_countdown_enabled = 1;
        game_engine_globals.time_until_map_cycle_timeout_expires_seconds = (float)gMapCycleTimeoutSeconds;
    }
}
