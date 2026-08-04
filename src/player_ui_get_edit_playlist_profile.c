/* player_ui_get_edit_playlist_profile @0x83699020 — return the in-progress edit buffer as a
 * game_variant, but only when the profile currently being edited is a playlist (multiplayer
 * variant) profile (saved_game_file_get_type == 1). Otherwise return null. */

#include <stdint.h>
#include "headers/player_ui_globals.h"
#include "headers/game_variant.h"

extern uint16_t saved_game_file_get_type(int index);

game_variant * player_ui_get_edit_playlist_profile(void)
{
    if ( saved_game_file_get_type(player_ui_globals.edit_profile_data.profile_index) != 1 )
        return 0;
    return (game_variant *)&player_ui_globals.edit_profile_data.edit_profile;
}
