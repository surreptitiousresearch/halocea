/* playlist_profile_change_name @0x83692F60 — menu handler: launch the virtual keyboard to edit the
 * playlist profile's human-readable description (max 24 wide chars / 0x30 bytes). If the keyboard
 * launched, constrain input to the profile-name validation set. Reports 0 when no profile is being
 * edited. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/game_variant.h"
#include "headers/virtual_keyboard_validate_mode.h"

extern game_variant * player_ui_get_edit_playlist_profile(void);
extern uint8_t virtual_keyboard_launch(uint16_t *text_buffer, uint16_t buffer_size, int16_t caption_index);
extern void virtual_keyboard_set_validation(int validate_mode);

uint8_t playlist_profile_change_name(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    game_variant *edit_playlist_profile = player_ui_get_edit_playlist_profile();
    if ( !edit_playlist_profile )
        return 0;

    if ( virtual_keyboard_launch(
            edit_playlist_profile->human_readable_game_description, 0x30, 9) )
        virtual_keyboard_set_validation(_virtual_keyboard_validate_playlist_profile_name);
    return 1;
}
