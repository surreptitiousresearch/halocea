/* multiplayer_type_menu_update_extended_description @0x8377E2F8 — refresh the multiplayer-game-type
 * list's extended-description panel. Finds the 0-based position of the currently focused child within
 * the list (0 if none of the children is focused, i.e. the loop runs to the end without a match), maps
 * that position to a description-icon frame index (or -1 to leave the title text untouched, for the
 * "no selection yet" and one specific mid-list position), stamps it onto the description's title text,
 * disables input on the first and fifth child widgets, and always stamps the active player's name onto
 * the description's third child widget.
 *
 * DEVIATION: the decompiler renders this as a `switch` on `child_position - 1` with several cases
 * sharing a goto'd block (LABEL_12 for positions 1/3, LABEL_15 for position 4 and any out-of-range
 * position). Collapsed here into equivalent grouped `case` labels — same set of (position -> string index)
 * mappings, just without the goto indirection.
 *
 * The switch constants are 1-based child positions of the tag-defined game-type list widget and indices
 * into its extended-description title string list — both live in ui.map tag data, not the executable;
 * enum_oracle lookups (2026-08-04) find no DB/headers_ref enum for either (the nearest family,
 * _description_*, does not match these values), so they stay numeric. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/player_profile.h"

extern void player_ui_get_active_player_profile(int16_t local_player_index, player_profile *profile);
extern void set_profile_name(widget_instance *text_widget, const wchar_t *name);

void multiplayer_type_menu_update_extended_description(widget_instance *list_widget)
{
    widget_instance *child = list_widget->children;
    __int16 focused_position = 0;
    for ( ; child; ++focused_position )
    {
        if ( child == list_widget->focused_child )
            break;
        child = child->next;
    }

    widget_instance *description_icon = list_widget->parameters.list_parameters.extended_description->children;
    __int16 title_string_index;

    switch ( (uint16_t)(focused_position - 1) )
    {
        case 0u:
        case 1u:
        case 2u:
            title_string_index = focused_position - 1;
            description_icon->animation_data.current_frame_index = 0;
            break;
        case 4u:
        case 5u:
            title_string_index = focused_position - 2;
            description_icon->animation_data.current_frame_index = 0;
            break;
        case 6u:
            title_string_index = focused_position - 2;
            description_icon->animation_data.current_frame_index = 2;
            break;
        case 3u:
        default:
            title_string_index = -1;
            break;
    }

    if ( title_string_index != -1 )
        description_icon->next->parameters.text_box_parameters.string_list_index = title_string_index;

    __int16 index = 0;
    for ( widget_instance *w = list_widget->children; w; w = w->next, ++index )
    {
        if ( index == 0 || index == 4 )
            w->never_receive_events = 1;
    }

    player_profile profile;
    player_ui_get_active_player_profile(0, &profile);
    set_profile_name(description_icon->next->next, profile.player_name);
}
