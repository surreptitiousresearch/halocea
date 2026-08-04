#include <stdint.h>
#include "headers/unit_hud_globals_definition.h"

/* recovered: (unit_hud_globals_definition *)((char *)unit_hud_globals + 88 * i) -> &unit_hud_globals->hud_states[i]
   (stride 88 == sizeof(unit_hud_state); decompiler mistyped return as unit_hud_globals_definition *) */
unit_hud_state * get_hud_state_0(int16_t local_player_index)
{
    return &unit_hud_globals->hud_states[local_player_index];
}
