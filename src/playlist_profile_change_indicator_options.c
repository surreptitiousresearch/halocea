/* playlist_profile_change_indicator_options @0x83694608 — UI event handler for the playlist-profile-edit
 * screen's three "indicator" list widgets (siblings: widget->parent->parent->children,
 * .next, .next->next), each hosting a list-type child widget (widget_type==2) whose selected
 * string_list_index drives the edited profile's universal_variant settings: 1st list selects goal_radar
 * (0/1/2, unselected/out-of-range values >= 3 leave it untouched); 2nd list toggles flags bits 0x1/0x40
 * together (0 -> bit0 only, 1 -> both bits, 2 -> neither, matching the game's usual
 * off/on/hidden-from-radar-style three-state list); 3rd list toggles flag bit 0x2 (0 -> set, 1 -> clear).
 * Returns 1 (handled) once player_ui_get_edit_playlist_profile() resolves an in-edit profile, 0 otherwise. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/game_variant.h"
#include "headers/ui_widget_type.h"
#include "headers/game_engine_universal_variant_flags.h"

#define _motion_tracker_flags ((1u << _allow_motion_tracker_bit) | (1u << _motion_tracker_no_enemies_bit))

extern game_variant * player_ui_get_edit_playlist_profile(void);

uint8_t playlist_profile_change_indicator_options(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    game_variant *edit_playlist_profile = player_ui_get_edit_playlist_profile();
    if ( !edit_playlist_profile )
        return 0;

    widget_instance *first_row = widget->parent->parent->children;

    widget_instance *goal_radar_widget;
    for ( goal_radar_widget = first_row->children; goal_radar_widget; goal_radar_widget = goal_radar_widget->next )
    {
        if ( goal_radar_widget->widget_type == _ui_widget_type_spinner_list )
            break;
    }
    unsigned int goal_radar_selection = (uint16_t)goal_radar_widget->parameters.text_box_parameters.string_list_index;
    if ( goal_radar_selection == 0 )
        edit_playlist_profile->universal_variant.goal_radar = 0;
    else if ( goal_radar_selection == 1 )
        edit_playlist_profile->universal_variant.goal_radar = 1;
    else if ( goal_radar_selection < 3 )
        edit_playlist_profile->universal_variant.goal_radar = 2;

    widget_instance *second_row = first_row->next;

    widget_instance *flags_a_widget;
    for ( flags_a_widget = second_row->children; flags_a_widget; flags_a_widget = flags_a_widget->next )
    {
        if ( flags_a_widget->widget_type == _ui_widget_type_spinner_list )
            break;
    }
    /* motion-tracker three-state: allow_motion_tracker (0x1) + motion_tracker_no_enemies (0x40) bits */
    unsigned int flags_a_selection = (uint16_t)flags_a_widget->parameters.text_box_parameters.string_list_index;
    if ( flags_a_selection )
    {
        if ( flags_a_selection == 1 )
            edit_playlist_profile->universal_variant.flags |= _motion_tracker_flags;
        else if ( flags_a_selection < 3 )
            edit_playlist_profile->universal_variant.flags &= ~_motion_tracker_flags;
    }
    else
    {
        edit_playlist_profile->universal_variant.flags =
            (edit_playlist_profile->universal_variant.flags & ~_motion_tracker_flags) | (1u << _allow_motion_tracker_bit);
    }

    widget_instance *third_row = second_row->next;

    widget_instance *flags_b_widget;
    for ( flags_b_widget = third_row->children; flags_b_widget; flags_b_widget = flags_b_widget->next )
    {
        if ( flags_b_widget->widget_type == _ui_widget_type_spinner_list )
            break;
    }
    /* friendly-navpoints toggle: universal_variant.flags bit 0x2 (allow_friendly_navpoints) */
    unsigned int flags_b_selection = (uint16_t)flags_b_widget->parameters.text_box_parameters.string_list_index;
    if ( flags_b_selection )
    {
        if ( flags_b_selection == 1 )
            edit_playlist_profile->universal_variant.flags &= ~(1u << _allow_friendly_navpoints_bit);
    }
    else
    {
        edit_playlist_profile->universal_variant.flags |= (1u << _allow_friendly_navpoints_bit);
    }

    return 1;
}
