/* player_ui_edit_profile_is_default_profile @0x83698E38 — true if the profile currently being
 * edited is a "default" profile. Only meaningful for controller/input profiles
 * (saved_game_file_get_type() < 2); the answer is bit 30 of the profile_index, which the profile
 * store uses to flag the built-in default slot. Game-variant slots (type >= 2) and the empty
 * sentinel (profile_index == -1) are never default. */

#include <stdint.h>
#include "headers/player_ui_globals.h"

extern uint16_t saved_game_file_get_type(int index);

uint8_t player_ui_edit_profile_is_default_profile(void)
{
    if ( player_ui_globals.edit_profile_data.profile_index == -1
      || saved_game_file_get_type(player_ui_globals.edit_profile_data.profile_index) >= 2u )
    {
        return 0;
    }

    return ((unsigned int)player_ui_globals.edit_profile_data.profile_index >> 30) & 1;
}
