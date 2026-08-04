/* game_load @0x83687A44 — load a map for play from the given options: log the RNG seed, precache the
 * map, copy the options into game globals, run the scenario load, and record whether the map loaded. */

#include <stdint.h>
#include "headers/game_globals.h"
#include <string.h>

extern void random_seed_debug_log(uint8_t enable);
extern void game_precache_new_map(const char *map_name, uint8_t blocking);
extern uint8_t scenario_load(const char *name);

uint8_t game_load(game_options *options)
{
    random_seed_debug_log(1u);
    game_precache_new_map(options->map_name, 1u);
    memcpy(&game_globals->options, options, sizeof(game_globals->options));
    if ( scenario_load(options->map_name) )
        game_globals->map_loaded = 1;
    return game_globals->map_loaded;
}
