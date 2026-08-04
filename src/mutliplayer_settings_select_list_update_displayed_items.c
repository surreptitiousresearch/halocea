/* mutliplayer_settings_select_list_update_displayed_items @0x83781828 — refresh a 3-wide spinner list of
 * game variant profiles: resolve which 3 profile indices are currently displayed, refresh the shared
 * variant-profile cache for them, then push each displayed item's cached variant into its child widget
 * (skipping the item once an index reads -1). Finally, if the list has an extended-description widget,
 * label it with the active local player's profile name. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/cached_variant_profile.h"
#include "headers/player_profile.h"

extern void spinner_list_3wide_determine_displayed_item_indices(widget_instance *list_widget, int *displayed_item_index);
extern void variant_profile_update_cache_for_nwide_list(int *profile_index, int number_of_profile_indices);
extern widget_instance *widget_instance_get_nth_child(widget_instance *widget, int n);
extern void multiplayer_settings_select_list_update_item(widget_instance *item, game_variant *variant);
extern void player_ui_get_active_player_profile(int16_t local_player_index, player_profile *profile);
extern void set_profile_name(widget_instance *text_widget, const wchar_t *name);

void mutliplayer_settings_select_list_update_displayed_items(widget_instance *list_widget)
{
    int displayed_item_index[3];
    spinner_list_3wide_determine_displayed_item_indices(list_widget, displayed_item_index);

    int profile_indices[3];
    for ( int i = 0; i < 3; i++ )
    {
        int item_index = displayed_item_index[i];
        profile_indices[i] = (item_index == -1) ? -1
            : ((int *)list_widget->parameters.list_parameters.list_items)[item_index];
    }
    variant_profile_update_cache_for_nwide_list(profile_indices, 3);

    for ( int i = 0; i < 3; i++ )
    {
        int item_index = displayed_item_index[i];
        if ( item_index == -1 )
            break;

        widget_instance *child = widget_instance_get_nth_child(list_widget, i);
        int profile_index = ((int *)list_widget->parameters.list_parameters.list_items)[item_index];

        _cached_variant_profile *cache_entry = cached_variant_profile;
        while ( cache_entry->profile_index != profile_index )
        {
            ++cache_entry;
            if ( cache_entry >= (_cached_variant_profile *)&object_list_header_data )
                goto next_item;
        }
        multiplayer_settings_select_list_update_item(child, &cache_entry->variant);

    next_item:;
    }

    if ( list_widget->parameters.list_parameters.extended_description )
    {
        player_profile profile;
        player_ui_get_active_player_profile(0, &profile);
        set_profile_name(list_widget->parameters.list_parameters.extended_description, profile.player_name);
    }
}
