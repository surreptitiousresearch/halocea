/* game_engine_initialize_for_new_map @0x83750328 — per-map bring-up of the active multiplayer game engine:
 * record whether the map is an original-Xbox MP map, verify spawn points, reset queued sounds and goals, run
 * the engine's own initialize callback (disposing the engine if it fails), and predict its resources. No-op
 * when no game engine is active. */

#include <stdint.h>
#include "headers/game_engine.h"
#include "headers/game_engine_globals.h"
#include "headers/blam_data_globals.h"
extern void *memset(void *dst, int value, unsigned int n);

/* game_engine: canonical decl from game_engine.h (via blam_data_globals.h) */

extern char *main_get_multiplayer_map_name(void);
extern int map_list_find_map(const char *map_name);
extern uint8_t map_list_is_map_original_xbox_mp_map(int index);
extern void netgame_verify_spawn_points(int16_t game_engine, int16_t team, int16_t count, char *string);
extern void game_engine_intialize_queued_sounds(void);
extern void game_engine_dispose(void);
extern void game_engine_predict_resources(void);

void game_engine_initialize_for_new_map(void)
{
    if ( !game_engine )
        return;

    int map = map_list_find_map(main_get_multiplayer_map_name());
    gGameEngineRunningOriginalMap = map_list_is_map_original_xbox_mp_map(map);

    /* six spawn-point verification passes; netgame_verify_spawn_points ignores its arguments */
    netgame_verify_spawn_points(0, 0, 0, 0);
    netgame_verify_spawn_points(0, 0, 0, 0);
    netgame_verify_spawn_points(0, 0, 0, 0);
    netgame_verify_spawn_points(0, 0, 0, 0);
    netgame_verify_spawn_points(0, 0, 0, 0);
    netgame_verify_spawn_points(0, 0, 0, 0);

    game_engine_intialize_queued_sounds();
    memset(global_goal, 0, sizeof(global_goal));
    timeout_for_endgame_sound = 0;
    game_engine_globals.current_team_to_add = 0;

    uint8_t (*initialize)(void) = game_engine->initialize;
    if ( initialize )
    {
        if ( !initialize() )
            game_engine_dispose();
    }
    game_engine_predict_resources();

    game_engine_globals.map_cycle_countdown_enabled = 0;
    game_engine_globals.time_until_map_cycle_timeout_expires_seconds = 0.0f;
    gForceServerToStop = 0;
}
