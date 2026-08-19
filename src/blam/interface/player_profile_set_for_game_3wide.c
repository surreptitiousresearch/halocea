/* player_profile_set_for_game_3wide @0x83692C38 — event handler for the 3-wide profile-select screen:
 * apply the profile highlighted in the widget's child list to the controller's single-player local player.
 * The list-item value is a saved-profile handle; NONE plays the error feedback sound, and a NON-NEGATIVE
 * value (valid saved-profile handles are negative) means a corrupt/placeholder entry — show deferred error
 * 31, play the error sound and delete the widget. Returns 1 only when the profile was loaded and applied. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/player_profile.h"
#include "headers/ui_audio_feedback_sound.h"
#include "headers/error_type.h"

extern void ui_play_audio_feedback_sound(int16_t audio_feedback);
extern void display_error_deferred(int16_t error_code, int16_t local_player_index, uint8_t modal, uint8_t pause_game_time);
extern uint8_t player_profile_get(int player_profile_index, player_profile *profile);
extern int16_t player_ui_get_single_player_local_player_from_controller(int16_t controller_index);
extern void player_ui_set_active_player_profile(int16_t local_player_index, int profile_index, player_profile *profile);

uint8_t player_profile_set_for_game_3wide(widget_instance *select_screen_widget, event_record *event, uint8_t *widget_deleted)
{
    widget_instance *list = select_screen_widget->children;
    int *list_items = (int *)list->parameters.list_parameters.list_items;
    int profile_index = list_items[list->parameters.text_box_parameters.string_list_index];

    if ( profile_index == -1 )
    {
        ui_play_audio_feedback_sound(_ui_audio_feedback_flag_failure);
        return 0;
    }
    if ( profile_index >= 0 )
    {
        display_error_deferred(_error_warning_saved_game_file_damaged, -1, 1u, 0);
        ui_play_audio_feedback_sound(_ui_audio_feedback_flag_failure);
        *widget_deleted = 1;
        return 0;
    }

    player_profile profile;
    if ( player_profile_get(profile_index, &profile) )
    {
        short local_player_index =
            player_ui_get_single_player_local_player_from_controller(event->controller_index);
        player_ui_set_active_player_profile(local_player_index,
                                            list_items[list->parameters.text_box_parameters.string_list_index],
                                            &profile);
        return 1;
    }
    return 0;
}
