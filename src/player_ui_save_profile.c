/* player_ui_save_profile @0x83699920 — save the profile currently being edited (player_ui_globals.
 * edit_profile_data), dispatched by the enumerated file's type: type 0 (none/new) just re-activates the
 * in-memory edit as the active profile without persisting; type 1 (playlist/game-variant profile) saves
 * it back to its existing slot, or — if the description changed from the original — allocates a new
 * playlist profile slot and saves there instead, updating profile_index to the new slot. On any successful
 * save, remembers the profile's enclosing directory as the last-used location. Always clears
 * edit_profile_data.profile_index to -1 before returning. Returns 1 if a save/reactivation happened, 0
 * (type not 0 or 1) otherwise. */

#include <wchar.h>
#include <stdint.h>
#include "headers/player_ui_globals.h"
#include "headers/game_variant_flags.h"

extern uint16_t saved_game_file_get_type(int index);
extern void player_ui_set_active_player_profile(int16_t local_player_index, int profile_index, player_profile *profile);
extern void playlist_profile_save(int playlist_profile_index, game_variant *variant);
extern uint8_t saved_game_file_get_path_to_enclosing_directory(int index, char *full_path);
extern void saved_game_file_remember_last_used_multiplayer_variant_directory(char *directory_path);
extern int ustrncmp(const wchar_t *string1, const wchar_t *string2, size_t count);
extern int playlist_profile_new(int16_t local_player_index, uint16_t *name);

int player_ui_save_profile(void)
{
    int saved = 0;
    int profile_index = player_ui_globals.edit_profile_data.profile_index;
    unsigned __int16 file_type = saved_game_file_get_type(profile_index);

    if ( !file_type )
    {
        player_ui_set_active_player_profile(0, profile_index,
            (player_profile *)&player_ui_globals.edit_profile_data.edit_profile);
        saved = 1;
    }
    else if ( file_type == 1 )
    {
        game_variant *edit_variant = (game_variant *)&player_ui_globals.edit_profile_data.edit_profile;
        game_variant *original_variant = (game_variant *)&player_ui_globals.edit_profile_data.original_profile;

        /* bit 0x40000000 in a saved-game index marks a not-yet-persisted (in-memory only) slot;
         * clear == an existing on-disk slot that can be saved back in place. No DB enum: runtime index tag. */
        if ( (profile_index & 0x40000000) == 0 )
        {
            playlist_profile_save(profile_index, edit_variant);

            char path[264];
            if ( saved_game_file_get_path_to_enclosing_directory(profile_index, path) )
                saved_game_file_remember_last_used_multiplayer_variant_directory(path);
            saved = 1;
        }
        else if ( ustrncmp(edit_variant->human_readable_game_description,
                       original_variant->human_readable_game_description, 0x18u) )
        {
            edit_variant->flags &= ~(1u << _variant_is_system_default_bit);
            int new_profile_index = playlist_profile_new(0, edit_variant->human_readable_game_description);
            if ( new_profile_index != -1 )
            {
                playlist_profile_save(new_profile_index, edit_variant);
                player_ui_globals.edit_profile_data.profile_index = new_profile_index;

                char path[264];
                if ( saved_game_file_get_path_to_enclosing_directory(new_profile_index, path) )
                    saved_game_file_remember_last_used_multiplayer_variant_directory(path);
            }
            saved = 1;
        }
    }

    player_ui_globals.edit_profile_data.profile_index = -1;
    return saved;
}
