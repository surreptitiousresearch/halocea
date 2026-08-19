/* delete_player_profile_request @0x83696CF8 — widget callback: from the second child (the profile list),
 * resolve the selected item to its underlying profile index and stash it in delete_profile_index for the
 * confirmation flow. If nothing valid is selected, play the error feedback and abort. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/ui_audio_feedback_sound.h"
#include "headers/blam_data_globals.h"

extern int preview_list_get_list_index_for_item(int index);
extern void ui_play_audio_feedback_sound(int16_t audio_feedback);

uint8_t delete_player_profile_request(widget_instance *wrapper_widget, event_record *event, uint8_t *widget_deleted)
{
    widget_instance *list_widget = wrapper_widget->children->next;
    int list_index_for_item =
            preview_list_get_list_index_for_item(list_widget->parameters.list_parameters.selected_list_item_index);
    if ( list_index_for_item != -1 )
    {
        delete_profile_index = ((int *)list_widget->parameters.list_parameters.list_items)[list_index_for_item];
        if ( delete_profile_index != -1 )
            return 1;
    }
    ui_play_audio_feedback_sound(_ui_audio_feedback_flag_failure);
    return 0;
}
