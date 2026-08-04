/* playlist_profile_initialize_indicator_options @0x83695F80 — widget setup callback: initializes the
 * 3 game-settings summary text indicators on the playlist-profile editor (goal radar, friendly fire,
 * teams), each the first widget_type==2 (text box) child of widget's 1st/2nd/3rd child group, driven
 * from the currently-edited playlist profile's universal_variant settings. Returns false (without setting
 * any indicator) if there's no profile currently being edited. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/game_variant.h"
#include "headers/ui_widget_type.h"
#include "headers/game_engine_universal_variant_flags.h"

extern game_variant * player_ui_get_edit_playlist_profile(void);

uint8_t playlist_profile_initialize_indicator_options(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    game_variant *edit_playlist_profile = player_ui_get_edit_playlist_profile();
    if ( !edit_playlist_profile )
        return 0;

    widget_instance *goal_radar_group = widget->children;
    widget_instance *goal_radar_text = goal_radar_group->children;
    while ( goal_radar_text && goal_radar_text->widget_type != _ui_widget_type_spinner_list )
        goal_radar_text = goal_radar_text->next;

    unsigned int goal_radar = edit_playlist_profile->universal_variant.goal_radar;
    if ( goal_radar == 0 || goal_radar >= 3 )
        goal_radar_text->parameters.text_box_parameters.string_list_index = 0;
    else if ( goal_radar == 1 )
        goal_radar_text->parameters.text_box_parameters.string_list_index = 1;
    else
        goal_radar_text->parameters.text_box_parameters.string_list_index = 2;

    widget_instance *friendly_fire_group = goal_radar_group->next;
    widget_instance *friendly_fire_text = friendly_fire_group->children;
    while ( friendly_fire_text && friendly_fire_text->widget_type != _ui_widget_type_spinner_list )
        friendly_fire_text = friendly_fire_text->next;

    int flags = edit_playlist_profile->universal_variant.flags;
    if ( flags & (1u << _allow_motion_tracker_bit) )
        friendly_fire_text->parameters.text_box_parameters.string_list_index =
            ((flags & (1u << _allow_motion_tracker_bit)) != 0) && ((flags & (1u << _motion_tracker_no_enemies_bit)) != 0);
    else
        friendly_fire_text->parameters.text_box_parameters.string_list_index = 2;

    widget_instance *teams_group = friendly_fire_group->next;
    widget_instance *teams_text = teams_group->children;
    while ( teams_text && teams_text->widget_type != _ui_widget_type_spinner_list )
        teams_text = teams_text->next;

    teams_text->parameters.text_box_parameters.string_list_index =
        (edit_playlist_profile->universal_variant.flags & (1u << _allow_friendly_navpoints_bit)) == 0;

    return 1;
}
