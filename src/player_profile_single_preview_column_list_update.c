/* player_profile_single_preview_column_list_update @0x83781680 — per-frame refresh of the player-profile
 * picker's single-preview-column list widget: rebuilds the list itself, updates the description panel's
 * name, and then either (a) fills the description panel from the selected profile's data when a real
 * profile row is selected, or (b) resets the description panel to its "no profile selected" (create-new)
 * placeholder state, hiding/showing a fixed set of child icon widgets by structural position — the DB has
 * no field names for these icons, so they are named by inferred role; confirmed field offsets via disasm
 * (visible@0x10, never_receive_events@0x12, alpha_modifier@0x24, animation_data.current_frame_index@0x58,
 * all against widget_instance's already-resolved layout).
 *
 * DEVIATION: Hex-Rays renders the final "hide the 7th chained widget" write as `*(_BYTE *)(v13+16) = 0`
 * after casting v13 (a real widget_instance*, r21) to int — a spurious cast artifact; disasm confirms r21
 * never leaves pointer form, reproduced here as a direct `->visible = 0` write. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/player_profile.h"

extern uint8_t player_profile_get_string(uint16_t *out, int index, void *list_items);
extern void single_preview_column_list_update(widget_instance *widget,
        uint8_t (*get_string)(uint16_t *, int, void *));
extern void player_ui_get_active_player_profile(int16_t local_player_index, player_profile *profile);
extern void set_profile_name(widget_instance *text_widget, const wchar_t *name);
extern uint16_t player_profile_number_of_available_primary_colors(void);
extern void * preview_list_get_data_for_item(int index);
extern void player_profile_update_item(widget_instance *item, player_profile *profile);
extern widget_instance *widget_instance_get_nth_child(widget_instance *widget, int n);

void player_profile_single_preview_column_list_update(widget_instance *widget)
{
    player_profile active_profile;
    widget_instance *item_widget;
    widget_instance *bottom_row_widget;

    single_preview_column_list_update(widget, player_profile_get_string);

    player_ui_get_active_player_profile(0, &active_profile);
    set_profile_name(widget->parameters.list_parameters.extended_description->children,
            active_profile.player_name);

    item_widget = widget->parameters.list_parameters.extended_description->children->next;

    if ( widget->parameters.list_parameters.selected_list_item_index >= 0
            && widget->parameters.list_parameters.selected_list_item_index
                    < (int)widget->parameters.list_parameters.number_of_items )
    {
        player_profile_update_item(item_widget,
                (player_profile *)preview_list_get_data_for_item(
                        widget->parameters.list_parameters.selected_list_item_index));
    }
    else
    {
        widget_instance *placeholder_icon = item_widget->children;
        widget_instance *color_swatch = placeholder_icon->next;
        widget_instance *new_profile_prompt = color_swatch->next->children;
        widget_instance *stat_icon_1 = new_profile_prompt->next;
        widget_instance *stat_icon_2 = stat_icon_1->next;
        widget_instance *stat_icon_3 = stat_icon_2->next;
        widget_instance *stat_icon_4 = stat_icon_3->next;
        widget_instance *stat_icon_5 = stat_icon_4->next;

        placeholder_icon->visible = 0;
        color_swatch->animation_data.current_frame_index = player_profile_number_of_available_primary_colors();
        new_profile_prompt->visible = 1;
        stat_icon_1->visible = 0;
        stat_icon_2->visible = 0;
        stat_icon_3->visible = 0;
        stat_icon_4->visible = 0;
        stat_icon_5->visible = 0;
    }

    bottom_row_widget = widget_instance_get_nth_child(widget, 11)->children->next;
    if ( widget->parameters.list_parameters.number_of_items )
    {
        bottom_row_widget->never_receive_events = 0;
        bottom_row_widget->alpha_modifier = 1.0f;
    }
    else
    {
        bottom_row_widget->never_receive_events = 1;
        bottom_row_widget->alpha_modifier = 0.333f;
    }
}
