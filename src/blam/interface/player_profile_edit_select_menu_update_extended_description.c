/* player_profile_edit_select_menu_update_extended_description @0x8377F3F8 — find the focused child's
 * position in the list (capped to 8, else -1) and push it as the extended-description text box's
 * string_list_index / animation frame (frame 6 when at the 9th/"more" entry). */

#include <stdint.h>
#include "headers/widget_instance.h"

void player_profile_edit_select_menu_update_extended_description(widget_instance *list_widget)
{
    widget_instance *child = list_widget->children;
    int16_t i;

    for ( i = 0; child; ++i )
    {
        if ( child == list_widget->focused_child )
            break;
        child = child->next;
    }

    if ( i > 8 )
        i = -1;

    int16_t frame_index = i;
    if ( i != -1 )
    {
        widget_instance *text_box = list_widget->parameters.list_parameters.extended_description->children;
        widget_instance *next = text_box->next;

        text_box->parameters.text_box_parameters.string_list_index = i;
        if ( i == 8 )
            frame_index = 6;
        next->animation_data.current_frame_index = frame_index;
    }
}
