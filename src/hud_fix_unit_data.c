/* hud_fix_unit_data @0x836D6A58 — migrate a local player's unit-HUD runtime state from one
 * local-player slot to another (used when local player indices are remapped). Copies the whole
 * unit_hud_state record. */

#include <stdint.h>
#include "headers/unit_hud_globals_definition.h"

extern void *memcpy(void *dest, const void *src, unsigned int count);

void hud_fix_unit_data(int16_t old_local_player_index, int16_t new_local_player_index)
{
    memcpy(
        &unit_hud_globals->hud_states[new_local_player_index],
        &unit_hud_globals->hud_states[old_local_player_index],
        sizeof(unit_hud_globals->hud_states[new_local_player_index]));
}
