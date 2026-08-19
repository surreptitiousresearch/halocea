/* playlist_gametype_select_menu_update_extended_description @0x8377E290 */
#include <stdint.h>
#include "headers/widget_globals.h"

void playlist_gametype_select_menu_update_extended_description(widget_instance *list_widget)
{
    widget_instance *children = list_widget->children;
    int16_t index = 0;

    while ( children )
    {
        if ( children == list_widget->focused_child )
            break;
        children = children->next;
        index++;
    }

    if ( index > 4 )
        index = -1;

    if ( index != -1 )
    {
        widget_instance *first_child = list_widget->parameters.list_parameters.extended_description->children;
        widget_instance *next = first_child->next;

        first_child->parameters.text_box_parameters.string_list_index = index;
        next->animation_data.current_frame_index = index;
    }
}
