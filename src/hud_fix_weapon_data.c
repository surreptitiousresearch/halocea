/* hud_fix_weapon_data @0x836B05E0 — migrate a local player's weapon-HUD runtime state from one
 * local-player slot to another (used when local player indices are remapped, e.g. on a split-screen
 * player leaving). Copies the weapon flash-timing block and the crosshair overlay state.
 *
 * DEVIATION: the decompiler emits the weapon_hud_state copy as a hand-unrolled 10-int loop starting
 * at &hud_state[old-1].last_grenade_flash_time with pre-increment; that walks exactly the 40 bytes
 * of weapon_hud_state[old] into weapon_hud_state[new], i.e. hud_state[new] = hud_state[old]. */

#include <stdint.h>
#include <string.h>
#include "headers/weapon_hud_globals_definition.h"


void hud_fix_weapon_data(int16_t old_local_player_index, int16_t new_local_player_index)
{
    weapon_hud_globals_definition *globals = weapon_hud_globals;

    globals->hud_state[new_local_player_index] = globals->hud_state[old_local_player_index];
    memcpy(
        &globals->crosshair_state[new_local_player_index],
        &globals->crosshair_state[old_local_player_index],
        sizeof(globals->crosshair_state[new_local_player_index]));
}
