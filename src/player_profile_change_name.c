/* player_profile_change_name @0x83696458 — widget callback: if a profile is being edited, launch the
 * virtual keyboard to rename it (24-char buffer, caption 8). Returns whether the keyboard was launched. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/player_profile.h"

extern player_profile * player_ui_get_edit_player_profile(void);
extern uint8_t virtual_keyboard_launch(uint16_t *text_buffer, uint16_t buffer_size, int16_t caption_index);

uint8_t player_profile_change_name(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    player_profile *edit_player_profile = player_ui_get_edit_player_profile();
    return edit_player_profile && virtual_keyboard_launch(edit_player_profile->player_name, 0x18u, 8);
}
