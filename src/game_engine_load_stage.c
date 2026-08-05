/* game_engine_load_stage @0x837470D8 — applies the currently-selected stage (global_stage) as the active
 * map/variant, then resets the local map if no network game is active yet. Returns whether a network
 * game is active. */

/* blam_data_globals.h first: supplies wchar_t before dependent headers are parsed (migration) */
#include <stdint.h>
#include "headers/blam_data_globals.h"
#include "headers/play_stage.h"


extern void main_set_multiplayer_map_name(const char *name);
extern void game_set_game_variant(game_variant *variant);
extern uint8_t network_game_is_active(void);
extern void main_reset_map(void);

int game_engine_load_stage(void)
{
    main_set_multiplayer_map_name(global_stage.map_name);
    game_set_game_variant(&global_stage.game_variant);

    int active = network_game_is_active();
    if (!(uint8_t)active)
        main_reset_map();   /* void; active is 0 on this branch */
    return active;
}
