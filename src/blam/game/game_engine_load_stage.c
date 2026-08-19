/* game_engine_load_stage @0x837470D8 — applies the currently-selected stage (global_stage) as the active
 * map/variant, then resets the local map if no network game is active yet.
 *
 * DEVIATION: the decompiler threaded network_game_is_active()'s r3 out as a return value. The binary has
 * no r3 definition of its own on either exit path (the not-active path leaves r3 = main_reset_map's
 * scratch) and no caller consumes r3 — attested void. */

/* blam_data_globals.h first: supplies wchar_t before dependent headers are parsed (migration) */
#include <stdint.h>
#include "headers/blam_data_globals.h"
#include "headers/play_stage.h"


extern void main_set_multiplayer_map_name(const char *name);
extern void game_set_game_variant(game_variant *variant);
extern uint8_t network_game_is_active(void);
extern void main_reset_map(void);

void game_engine_load_stage(void)
{
    main_set_multiplayer_map_name(global_stage.map_name);
    game_set_game_variant(&global_stage.game_variant);

    if (!network_game_is_active())
        main_reset_map();
}
