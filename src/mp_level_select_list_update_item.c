/* mp_level_select_list_update_item @0x8377F6C8 — refreshes one row of the multiplayer level-select list
 * widget: the row's three children (map preview picture, animated highlight, and map name text) are all
 * driven by the same map-list pic-name index. */

#include <stdint.h>
#include "headers/widget_instance.h"

extern int16_t map_list_get_pic_name_index(int index);

void mp_level_select_list_update_item(widget_instance *item, int index)
{
    widget_instance *picture = item->children;
    widget_instance *highlight = picture->next;
    widget_instance *name = highlight->next;

    picture->parameters.text_box_parameters.string_list_index = map_list_get_pic_name_index(index);
    highlight->animation_data.current_frame_index = map_list_get_pic_name_index(index);
    name->parameters.text_box_parameters.string_list_index = map_list_get_pic_name_index(index);
}
