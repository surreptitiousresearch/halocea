/* player_ui_is_profile_name_original_name @0x83699070 */
#include "headers/player_ui_globals.h"

extern int ustrcmp(const wchar_t *string1, const wchar_t *string2);

/* True when a profile edit is in progress and `new_name` matches the name the
 * profile started with (i.e. the user has not actually renamed it). */
BOOL player_ui_is_profile_name_original_name(const wchar_t *new_name)
{
    return player_ui_globals.edit_profile_data.profile_index != -1
        && ustrcmp(new_name,
               player_ui_globals.edit_profile_data.original_profile.player_profile.player_name) == 0;
}
