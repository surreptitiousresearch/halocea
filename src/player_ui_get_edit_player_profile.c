/* player_ui_get_edit_player_profile @0x83698FD8 — return the in-progress edit buffer as a
 * player_profile, but only when the profile currently being edited is a controller/player
 * profile (saved_game_file_get_type == 0). Otherwise return null. */

#include <stdint.h>
#include "headers/player_ui_globals.h"
#include "headers/player_profile.h"

extern uint16_t saved_game_file_get_type(int index);

player_profile * player_ui_get_edit_player_profile(void)
{
    if ( saved_game_file_get_type(player_ui_globals.edit_profile_data.profile_index) != 0 )
        return 0;
    return (player_profile *)&player_ui_globals.edit_profile_data.edit_profile;
}
