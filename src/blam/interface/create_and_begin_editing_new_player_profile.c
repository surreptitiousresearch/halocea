/* create_and_begin_editing_new_player_profile @0x83697078 — widget callback: create a new untitled player
 * profile for the event's controller (defaulting to controller 0 when unset), begin editing it, name it
 * from the generated placeholder, and launch the virtual keyboard to let the user rename it. On any
 * failure (no available name, allocation failure, no edit profile), plays the UI error feedback instead. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/player_profile.h"
#include "headers/ui_audio_feedback_sound.h"
#include "headers/error_type.h"

extern void saved_game_file_get_useable_untitled_profile_name(uint16_t *display_name);
extern int player_profile_new(int16_t local_player_index, const uint16_t *name);
extern void player_ui_begin_editing_profile(int profile_index);
extern player_profile * player_ui_get_edit_player_profile(void);
extern void player_ui_set_active_player_profile(int16_t local_player_index, int profile_index, player_profile *profile);
extern void player_ui_end_editing_profile(void);
extern void ustrncpy(wchar_t *dest, const wchar_t *src, unsigned int count);
extern uint8_t virtual_keyboard_launch(uint16_t *text_buffer, uint16_t buffer_size, int16_t caption_index);
extern void display_error_deferred(int16_t error_code, int16_t local_player_index, uint8_t modal, uint8_t pause_game_time);
extern void ui_play_audio_feedback_sound(int16_t audio_feedback);

uint8_t create_and_begin_editing_new_player_profile(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    int keyboard_launched = 0;
    int16_t controller_index = event->controller_index == -1 ? 0 : event->controller_index;
    unsigned short generated_name[144];

    saved_game_file_get_useable_untitled_profile_name(generated_name);
    if ( generated_name[0] )
    {
        int profile_index = player_profile_new(controller_index, generated_name);
        if ( profile_index != -1 )
        {
            player_ui_begin_editing_profile(profile_index);
            player_profile *edit_profile = player_ui_get_edit_player_profile();
            player_ui_set_active_player_profile(0, profile_index, edit_profile);
            if ( edit_profile )
            {
                ustrncpy(edit_profile->player_name, generated_name, 11u);
                edit_profile->player_name[11] = 0;
                keyboard_launched = virtual_keyboard_launch(edit_profile->player_name, 0x18u, 8);
            }
            else
            {
                player_ui_end_editing_profile();
            }
        }
    }

    if ( !(uint8_t)keyboard_launched )
    {
        display_error_deferred(_error_unable_to_create_player_profile, -1, 1u, 0);
        ui_play_audio_feedback_sound(_ui_audio_feedback_flag_failure);
    }
    return keyboard_launched;
}
