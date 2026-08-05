/* solo_map_single_preview_column_list_update @0x83781388 — refresh a solo-map single-column list widget,
 * then stamp the active local player's profile name and the currently selected item into the extended
 * description panel. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/player_profile.h"

extern void single_preview_column_list_update(widget_instance *widget,
    uint8_t (__fastcall *func)(uint16_t *, int, void *));
extern uint8_t solo_map_get_string(uint16_t *out, int index, void *list_items);
extern void player_ui_get_active_player_profile(int16_t local_player_index, player_profile *profile);
extern void set_profile_name(widget_instance *text_widget, const wchar_t *name);
extern void solo_level_select_list_update_item(widget_instance *item, int index);

void solo_map_single_preview_column_list_update(widget_instance *widget)
{
    player_profile profile;

    single_preview_column_list_update(widget, solo_map_get_string);
    player_ui_get_active_player_profile(0, &profile);
    set_profile_name(widget->parameters.list_parameters.extended_description->children, profile.player_name);

    int selected_list_item_index = widget->parameters.list_parameters.selected_list_item_index;
    widget_instance *next = widget->parameters.list_parameters.extended_description->children->next;

    next->animation_data.current_frame_index = 0;
    solo_level_select_list_update_item(next, selected_list_item_index);
}
