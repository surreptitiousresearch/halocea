/* game_engine_allow_dynamic_lighting @0x8374A290 — true unless a game engine is running that either forbids
 * dynamic lighting (flag bit 0) or the rasterizer effects level is turned down to 1 or lower. With no game
 * engine active dynamic lighting is always permitted. */

#include <stdint.h>
#include "headers/game_engine.h"
#include "headers/game_engine_flags.h"
#include "headers/game_engine_globals.h"
#include "headers/rasterizer_debug_options_struct.h"

uint8_t game_engine_allow_dynamic_lighting(void)
{
    if (!game_engine)
        return 1;
    if ((game_engine_globals.flags & (1u << _game_engine_disable_dynamic_light_bit)) != 0)
        return 0;
    if (rasterizer_debug_options.rasterizer_effects_level <= 1)
        return 0;
    return 1;
}
