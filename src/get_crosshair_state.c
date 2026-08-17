/* get_crosshair_state @0x836AE9D8 */
#include <stdint.h>
#include "headers/weapon_hud_globals_definition.h"

crosshair_hud_state * get_crosshair_state(int16_t local_player_index)
{
    return &weapon_hud_globals->crosshair_state[local_player_index];
}
