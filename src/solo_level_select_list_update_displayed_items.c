/* solo_level_select_list_update_displayed_items @0x837817A0 — refreshes the solo-campaign level-select
 * spinner list widget: looks up the active local player's profile (name + highest-completed-level marker),
 * sets the profile name on the list's description widget, determines which 3 level indices the spinner
 * should currently show, and updates each of the list's 3 child item widgets to display them. */

#include <wchar.h>
#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/player_profile.h"

extern void player_ui_get_active_player_profile(int16_t local_player_index, player_profile *profile);
extern void player_profile_get_highest_completed_solo_level(player_profile *profile, int16_t *level, int16_t *difficulty);
extern void set_profile_name(widget_instance *text_widget, const wchar_t *name);
extern void spinner_list_3wide_determine_displayed_item_indices(widget_instance *list_widget, int *displayed_item_index);
extern widget_instance *widget_instance_get_nth_child(widget_instance *widget, int index);
extern void solo_level_select_list_update_item(widget_instance *item, int index);

void solo_level_select_list_update_displayed_items(widget_instance *list_widget)
{
    player_profile profile;
    int16_t difficulty;
    int16_t map_ids[3];
    int displayed_indices[6];

    player_ui_get_active_player_profile(0, &profile);
    player_profile_get_highest_completed_solo_level(&profile, &difficulty, map_ids);
    set_profile_name(list_widget->parameters.list_parameters.extended_description, profile.player_name);
    spinner_list_3wide_determine_displayed_item_indices(list_widget, displayed_indices);

    for ( int i = 0; i < 3 && displayed_indices[i] != -1; ++i )
    {
        widget_instance *item_widget = widget_instance_get_nth_child(list_widget, i);
        solo_level_select_list_update_item(item_widget, displayed_indices[i]);
    }
}
