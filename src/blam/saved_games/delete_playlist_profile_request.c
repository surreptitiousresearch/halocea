/* delete_playlist_profile_request @0x83696D78 — widget callback: resolve the selected list item to a
 * playlist (game-settings) profile index and stash it in delete_profile_index. Default/built-in settings
 * carry the 0x40000000 flag and cannot be deleted — reject those with an error. Play the failure sound and
 * abort on any invalid selection. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/ui_audio_feedback_sound.h"
#include "headers/error_type.h"
#include "headers/blam_data_globals.h"

extern int preview_list_get_list_index_for_item(int index);
extern void ui_play_audio_feedback_sound(int16_t audio_feedback);
extern void display_error_deferred(int16_t error_code, int16_t local_player_index, uint8_t modal, uint8_t pause_game_time);

uint8_t delete_playlist_profile_request(widget_instance *wrapper_widget, event_record *event, uint8_t *widget_deleted)
{
    delete_profile_index = ((int *)wrapper_widget->parameters.list_parameters.list_items)
            [preview_list_get_list_index_for_item(wrapper_widget->parameters.list_parameters.selected_list_item_index)];
    if ( delete_profile_index == -1 )
    {
        ui_play_audio_feedback_sound(_ui_audio_feedback_flag_failure);
    }
    else
    {
        if ( (delete_profile_index & 0x40000000) == 0 )
            return 1;
        ui_play_audio_feedback_sound(_ui_audio_feedback_flag_failure);
        display_error_deferred(_error_cannot_delete_default_game_settings, -1, 1u, 0);
    }
    return 0;
}
