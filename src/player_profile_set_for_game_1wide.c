/* player_profile_set_for_game_1wide @0x83692D00 — profile chooser accept handler: find the list
 * child (widget_type 2) under the given parent, read the highlighted profile id. A non-negative id
 * is a built-in profile that cannot be assigned (deferred error, negative feedback). A negative id
 * is loaded and set as the active player profile for this event's controller. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/player_profile.h"
#include "headers/ui_audio_feedback_sound.h"
#include "headers/ui_widget_type.h"
#include "headers/error_type.h"

extern void display_error_deferred(int16_t error_code, int16_t local_player_index, uint8_t modal, uint8_t pause_game_time);
extern void ui_play_audio_feedback_sound(int16_t audio_feedback);
extern uint8_t player_profile_get(int player_profile_index, player_profile *profile);
extern void player_ui_set_active_player_profile(int16_t local_player_index, int profile_index, player_profile *profile);

uint8_t player_profile_set_for_game_1wide(widget_instance *list_widgets_parent, event_record *event, uint8_t *widget_deleted)
{
    __int16 local_player_index = event->controller_index;

    widget_instance *list_child = list_widgets_parent->children;
    for ( ; list_child; list_child = list_child->next )
    {
        if ( list_child->widget_type == _ui_widget_type_spinner_list )
            break;
    }

    int *list_items = (int *)list_child->parameters.list_parameters.list_items;
    int profile_index = list_items[list_child->parameters.text_box_parameters.string_list_index];
    if ( profile_index >= 0 )
    {
        display_error_deferred(_error_warning_saved_game_file_damaged, event->controller_index, 1, 0);
        ui_play_audio_feedback_sound(_ui_audio_feedback_flag_failure);
        return 0;
    }

    player_profile profile;
    if ( !player_profile_get(profile_index, &profile) )
        return 0;

    player_ui_set_active_player_profile(local_player_index,
        list_items[list_child->parameters.text_box_parameters.string_list_index], &profile);
    return 1;
}
