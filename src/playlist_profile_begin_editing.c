/* playlist_profile_begin_editing @0x83692DC0 — profile-list accept handler: resolve the highlighted
 * preview item to a profile id. A negative (non -1) id is a valid, non-built-in profile and editing
 * begins; a non-negative id is a built-in profile that cannot be edited (deferred error). -1 or the
 * error path plays the negative feedback sound and reports not-handled. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/ui_audio_feedback_sound.h"
#include "headers/blam_data_globals.h"
#include "headers/error_type.h"

extern int preview_list_get_list_index_for_item(int index);
extern void player_ui_begin_editing_profile(int profile_index);
extern void display_error_deferred(int16_t error_code, int16_t local_player_index, uint8_t modal, uint8_t pause_game_time);
extern void ui_play_audio_feedback_sound(int16_t audio_feedback);

uint8_t playlist_profile_begin_editing(widget_instance *wrapper_widget, event_record *event, uint8_t *widget_deleted)
{
    int16_t selected = wrapper_widget->parameters.list_parameters.selected_list_item_index;
    delete_profile_index = -1;

    int profile_index = ((int *)wrapper_widget->parameters.list_parameters.list_items)
                            [preview_list_get_list_index_for_item(selected)];
    if ( profile_index != -1 )
    {
        if ( profile_index < 0 )
        {
            player_ui_begin_editing_profile(profile_index);
            return 1;
        }
        display_error_deferred(_error_warning_saved_game_file_damaged, -1, 1, 0);
    }
    ui_play_audio_feedback_sound(_ui_audio_feedback_flag_failure);
    return 0;
}
