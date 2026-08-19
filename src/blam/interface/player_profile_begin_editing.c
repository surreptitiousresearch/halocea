/* player_profile_begin_editing @0x83696340 — widget callback: from the profile list, resolve the selected
 * item to a profile index; if it names an existing (non-default) profile begin editing it, otherwise report
 * that no profile is selected and play the error feedback sound. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/ui_audio_feedback_sound.h"
#include "headers/error_type.h"
#include "headers/blam_data_globals.h"

extern void player_ui_begin_editing_profile(int profile_index);
extern void display_error_deferred(int16_t error_code, int16_t local_player_index, uint8_t modal, uint8_t pause_game_time);
extern void ui_play_audio_feedback_sound(int16_t audio_feedback);

uint8_t player_profile_begin_editing(widget_instance *wrapper_widget, event_record *event, uint8_t *widget_deleted)
{
    widget_instance *children = wrapper_widget->children;
    int *list_items = (int *)children->parameters.list_parameters.list_items;
    int string_list_index = children->parameters.text_box_parameters.string_list_index;
    int profile_index;

    delete_profile_index = -1;
    profile_index = list_items[string_list_index];
    if ( profile_index != -1 )
    {
        if ( profile_index < 0 )
        {
            player_ui_begin_editing_profile(profile_index);
            return 1;
        }
        display_error_deferred(_error_warning_saved_game_file_damaged, -1, 1u, 0);
    }
    ui_play_audio_feedback_sound(_ui_audio_feedback_flag_failure);
    return 0;
}
