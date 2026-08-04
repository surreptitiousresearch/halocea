#include <stdint.h>
#include "headers/weapon_hud_globals_definition.h"
#include "headers/blam_data_globals.h"


weapon_hud_globals_definition * get_hud_state(int16_t local_player_index)
{
    /* recovered: (char *)weapon_hud_globals + 40*local_player_index -> &hud_state[local_player_index] (stride 40 = sizeof weapon_hud_state, at offset 0) */
    return (weapon_hud_globals_definition *)&weapon_hud_globals->hud_state[local_player_index];
}
