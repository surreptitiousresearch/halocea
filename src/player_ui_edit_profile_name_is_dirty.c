/* player_ui_edit_profile_name_is_dirty @0x83698EA0 — true if the user has changed the *name*
 * (only the name, not the rest of the profile) of the edit slot relative to its original snapshot.
 * The type selects which slot view and which name field to compare:
 *   type == 1 : game variant  → human_readable_game_description[24] (24 wide chars)
 *   type == 0 : player profile → player_name[12] (12 wide chars)
 * Any other type (>=2) is not name-editable and reports clean. */

#include <wchar.h>
#include <stdint.h>
#include "headers/player_ui_globals.h"

extern uint16_t saved_game_file_get_type(int index);
extern int ustrncmp(const wchar_t *string1, const wchar_t *string2, size_t count);

uint8_t player_ui_edit_profile_name_is_dirty(void)
{
    if ( player_ui_globals.edit_profile_data.profile_index != -1 )
    {
        uint16_t saved_game_type = saved_game_file_get_type(player_ui_globals.edit_profile_data.profile_index);
        if ( saved_game_type )
        {
            if ( saved_game_type == 1
              && ustrncmp(
                     player_ui_globals.edit_profile_data.edit_profile.variant.human_readable_game_description,
                     player_ui_globals.edit_profile_data.original_profile.variant.human_readable_game_description,
                     0x18u) )
            {
                return 1;
            }
        }
        else
        {
            if ( ustrncmp(
                     player_ui_globals.edit_profile_data.edit_profile.player_profile.player_name,
                     player_ui_globals.edit_profile_data.original_profile.player_profile.player_name,
                     0xCu) )
            {
                return 1;
            }
        }
    }

    return 0;
}
