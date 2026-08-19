/* get_hud_state @0x836AE9B8 */
/* DEVIATION: return type corrected to weapon_hud_state* — the binary returns
 * weapon_hud_globals + 40*local_player_index (0x836AE9C0 slwi/add x5, 0x836AE9CC slwi 3 = *40),
 * i.e. &hud_state[local_player_index] (stride 40 = sizeof(weapon_hud_state)); a
 * weapon_hud_globals_definition* is wrong for index 1 (mid-struct). */
#include <stdint.h>
#include "headers/weapon_hud_globals_definition.h"
#include "headers/blam_data_globals.h"


weapon_hud_state * get_hud_state(int16_t local_player_index)
{
    return &weapon_hud_globals->hud_state[local_player_index];
}
