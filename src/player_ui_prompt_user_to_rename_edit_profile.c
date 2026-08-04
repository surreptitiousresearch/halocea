/* player_ui_prompt_user_to_rename_edit_profile @0x83698F38 — launches the on-screen (virtual)
 * keyboard to rename the profile currently being edited, targeting the name field appropriate to
 * the slot type:
 *   type == 0 : player profile → player_name buffer, capacity 24 wide chars, caption index 10
 *   type == 1 : game variant  → human_readable_game_description buffer, capacity 48, caption 10,
 *               and (if the keyboard actually opened) sets validation mode 2 (game-name rules)
 * Any other type, or the empty sentinel, does nothing and returns 0. Returns the keyboard-launch
 * result. */

#include <stdint.h>
#include "headers/player_ui_globals.h"
#include "headers/virtual_keyboard_validate_mode.h"

extern uint16_t saved_game_file_get_type(int index);
extern uint8_t virtual_keyboard_launch(uint16_t *text_buffer, uint16_t buffer_size, int16_t caption_index);
extern void virtual_keyboard_set_validation(int validate_mode);

int player_ui_prompt_user_to_rename_edit_profile(void)
{
    unsigned __int16 saved_game_type;
    uint8_t launched;

    if ( player_ui_globals.edit_profile_data.profile_index == -1 )
        return 0;

    saved_game_type = saved_game_file_get_type(player_ui_globals.edit_profile_data.profile_index);
    if ( !saved_game_type )
    {
        return virtual_keyboard_launch(
                   player_ui_globals.edit_profile_data.edit_profile.player_profile.player_name,
                   0x18u,
                   10);
    }

    if ( saved_game_type != 1 )
        return 0;

    launched = virtual_keyboard_launch(
                   player_ui_globals.edit_profile_data.edit_profile.variant.human_readable_game_description,
                   0x30u,
                   10);
    if ( launched )
        virtual_keyboard_set_validation(_virtual_keyboard_validate_playlist_profile_name);

    return launched;
}
