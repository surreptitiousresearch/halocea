/* mp_map_single_preview_column_list_update @0x83781458 — refreshes a single-column map-preview list widget:
 * runs the generic single-preview-column update, stamps the active player's name onto the extended
 * description's child text widget, then sets the pic-name string/frame index on the extended
 * description's three preview sub-widgets (name, description-frame, and gametype-name text). */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/player_profile.h"

extern void single_preview_column_list_update(widget_instance *widget, uint8_t (*func)(uint16_t *, int, void *));
extern uint8_t mp_map_get_string(uint16_t *out, int index, void *list_items);
extern void player_ui_get_active_player_profile(int16_t local_player_index, player_profile *profile);
extern void set_profile_name(widget_instance *text_widget, const wchar_t *name);
extern int16_t map_list_get_pic_name_index(int index);

void mp_map_single_preview_column_list_update(widget_instance *widget)
{
    single_preview_column_list_update(widget, (uint8_t (*)(uint16_t *, int, void *))mp_map_get_string);

    player_profile profile;
    player_ui_get_active_player_profile(0, &profile);
    set_profile_name(widget->parameters.list_parameters.extended_description->children, profile.player_name);

    widget_instance *extended_description = widget->parameters.list_parameters.extended_description;
    int selected_list_item_index = widget->parameters.list_parameters.selected_list_item_index;

    widget_instance *name_widget = extended_description->children->next->children;
    widget_instance *frame_widget = name_widget->next;
    widget_instance *gametype_widget = frame_widget->next;

    name_widget->parameters.text_box_parameters.string_list_index = map_list_get_pic_name_index(selected_list_item_index);
    frame_widget->animation_data.current_frame_index = map_list_get_pic_name_index(selected_list_item_index);
    gametype_widget->parameters.text_box_parameters.string_list_index = map_list_get_pic_name_index(selected_list_item_index);
}
