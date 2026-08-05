/* player_profile_color_picker_update @0x83781D70 — refresh the color-picker list widget, then stamp the
 * active local player's profile name and selected color swatch into the extended description panel.
 * Same shape as solo_map_single_preview_column_list_update.c, but the swatch text box is one level deeper
 * (extended_description->children->next->children). */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/player_profile.h"

extern void single_preview_column_list_update(widget_instance *widget,
    uint8_t (__fastcall *func)(uint16_t *, int, void *));
extern uint8_t color_picker_get_string(uint16_t *out, int index, void *list_items);
extern void player_ui_get_active_player_profile(int16_t local_player_index, player_profile *profile);
extern void set_profile_name(widget_instance *text_widget, const wchar_t *name);

void player_profile_color_picker_update(widget_instance *list_widget)
{
    player_profile profile;

    single_preview_column_list_update(list_widget, color_picker_get_string);
    player_ui_get_active_player_profile(0, &profile);
    set_profile_name(list_widget->parameters.list_parameters.extended_description->children, profile.player_name);

    int16_t selected_list_item_index = list_widget->parameters.list_parameters.selected_list_item_index;
    widget_instance *swatch = list_widget->parameters.list_parameters.extended_description->children->next->children;
    widget_instance *next = swatch->next;

    swatch->parameters.text_box_parameters.string_list_index = selected_list_item_index;
    next->animation_data.current_frame_index = selected_list_item_index;
}
