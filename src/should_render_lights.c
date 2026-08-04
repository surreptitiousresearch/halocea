/* should_render_lights @ 0x836F7990 — true when the lighting master switch is
 * enabled and the current game engine permits dynamic lighting. */

#include <stdint.h>
#include "headers/lights_game_globals.h"

extern uint8_t game_engine_allow_dynamic_lighting(void);

int should_render_lights(void)
{
    if (!((lights_game_globals_struct *)lights_game_globals)->render_lights)
        return 0;
    if (!game_engine_allow_dynamic_lighting())
        return 0;
    return 1;
}
