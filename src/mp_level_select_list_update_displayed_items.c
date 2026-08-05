/* mp_level_select_list_update_displayed_items @0x83781E00 — refreshes the multiplayer map-select spinner
 * list: stamps the active player's name on the description widget, determines which 3 map indices the
 * spinner should show, and updates each shown item's 3 sub-widgets (pic name text, frame icon, gametype name
 * text) to match. Mirrors solo_level_select_list_update_displayed_items.c's structure, inlining the per-item
 * update instead of delegating to a helper. */

#include <wchar.h>
#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/player_profile.h"

extern void player_ui_get_active_player_profile(int16_t local_player_index, player_profile *profile);
extern void set_profile_name(widget_instance *text_widget, const wchar_t *name);
extern void spinner_list_3wide_determine_displayed_item_indices(widget_instance *list_widget, int *displayed_item_index);
extern widget_instance *widget_instance_get_nth_child(widget_instance *widget, int n);
extern int16_t map_list_get_pic_name_index(int index);

void mp_level_select_list_update_displayed_items(widget_instance *list_widget)
{
    player_profile profile;
    int displayed_indices[6];

    player_ui_get_active_player_profile(0, &profile);
    set_profile_name(list_widget->parameters.list_parameters.extended_description, profile.player_name);
    spinner_list_3wide_determine_displayed_item_indices(list_widget, displayed_indices);

    for ( int i = 0; i < 3 && displayed_indices[i] != -1; i++ )
    {
        int map_index = displayed_indices[i];
        widget_instance *pic_name = widget_instance_get_nth_child(list_widget, i)->children;
        widget_instance *icon = pic_name->next;
        widget_instance *gametype_name = icon->next;

        pic_name->parameters.text_box_parameters.string_list_index = map_list_get_pic_name_index(map_index);
        icon->animation_data.current_frame_index = map_list_get_pic_name_index(map_index);
        gametype_name->parameters.text_box_parameters.string_list_index = map_list_get_pic_name_index(map_index);
    }
}
