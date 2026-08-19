/* lights_enable @0x836F7AF8 — toggle dynamic-light rendering and return the new state. */

#include <stdint.h>
#include "headers/lights_game_globals_t.h"

uint8_t lights_enable(uint8_t enable)
{
    ((lights_game_globals_t *)lights_game_globals)->render_lights = enable;
    return enable;
}
