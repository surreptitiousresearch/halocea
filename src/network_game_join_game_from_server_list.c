/* network_game_join_game_from_server_list @0x836921E0 — server-browser list accept handler: if the
 * list has focus and the highlighted row is a valid, non-open entry (advertised_game_data.open != 1),
 * play the negative feedback sound. Returns 0 (event not consumed for navigation). The list_items
 * array is displayed_servers (advertised_game_data*[]), wired up by server_list_menu_update. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/advertised_game_data.h"
#include "headers/event_record.h"
#include "headers/ui_audio_feedback_sound.h"

extern void ui_play_audio_feedback_sound(int16_t audio_feedback);

uint8_t network_game_join_game_from_server_list(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    if ( widget->focused_child )
    {
        int16_t string_list_index = widget->parameters.text_box_parameters.string_list_index;
        if ( string_list_index >= 0 && string_list_index < widget->parameters.list_parameters.number_of_items )
        {
            advertised_game_data **list_items = (advertised_game_data **)widget->parameters.list_parameters.list_items;
            if ( list_items )
            {
                if ( widget->parameters.list_parameters.number_of_items
                    && list_items[string_list_index]->open != 1 )
                    ui_play_audio_feedback_sound(_ui_audio_feedback_flag_failure);
            }
        }
    }
    return 0;
}
