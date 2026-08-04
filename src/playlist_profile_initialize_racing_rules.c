/* playlist_profile_initialize_racing_rules @0x83695660 — UI init callback (sibling of
 * playlist_profile_initialize_koth_rules / _slayer_rules): when the Racing rules screen is first shown, seed its
 * five sibling text-box selections from the edited game variant. Box 1 maps ctf.single_flag_time laps
 * (0/1/2+ -> 0/1/2). Box 2 maps race.race_type (0/1/2+ -> 0/1/2). Box 3 maps score_to_win
 * (3/5/10/15/25 -> 1/2/3/4/5, else 0). Box 4 shows teams on/off. Box 5 maps time_limit in ticks
 * (18000/27000/36000/45000/54000/81000 -> 1/2/3/4/5/6, anything else including 0 -> 0). Each text box is the
 * first widget_type==2 descendant of successive sibling groups. Returns 1, or 0 if no profile is being edited. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/game_variant.h"
#include "headers/ui_widget_type.h"

extern game_variant * player_ui_get_edit_playlist_profile(void);

static widget_instance *first_text_box(widget_instance *group)
{
    for ( widget_instance *child = group->children; child; child = child->next )
    {
        if ( child->widget_type == _ui_widget_type_spinner_list )
            return child;
    }
    return nullptr;
}

uint8_t playlist_profile_initialize_racing_rules(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    game_variant *edit_playlist_profile = player_ui_get_edit_playlist_profile();
    if ( !edit_playlist_profile )
        return 0;

    widget_instance *group = widget->children;

    widget_instance *laps_box = first_text_box(group);
    unsigned int single_flag_time = edit_playlist_profile->game_engine_variant.ctf.single_flag_time;
    if ( single_flag_time == 1 )
        laps_box->parameters.text_box_parameters.string_list_index = 1;
    else if ( single_flag_time && single_flag_time < 3 )
        laps_box->parameters.text_box_parameters.string_list_index = 2;
    else
        laps_box->parameters.text_box_parameters.string_list_index = 0;

    widget_instance *race_type_box = first_text_box(group->next);
    unsigned int race_type = edit_playlist_profile->game_engine_variant.race.race_type;
    if ( race_type == 1 )
        race_type_box->parameters.text_box_parameters.string_list_index = 1;
    else if ( race_type && race_type < 3 )
        race_type_box->parameters.text_box_parameters.string_list_index = 2;
    else
        race_type_box->parameters.text_box_parameters.string_list_index = 0;

    widget_instance *score_box = first_text_box(group->next->next);
    switch ( edit_playlist_profile->universal_variant.score_to_win )
    {
        case 3:  score_box->parameters.text_box_parameters.string_list_index = 1; break;
        case 5:  score_box->parameters.text_box_parameters.string_list_index = 2; break;
        case 10: score_box->parameters.text_box_parameters.string_list_index = 3; break;
        case 15: score_box->parameters.text_box_parameters.string_list_index = 4; break;
        case 25: score_box->parameters.text_box_parameters.string_list_index = 5; break;
        default: score_box->parameters.text_box_parameters.string_list_index = 0; break;
    }

    widget_instance *teams_box = first_text_box(group->next->next->next);
    if ( edit_playlist_profile->universal_variant.teams )
        teams_box->parameters.text_box_parameters.string_list_index = 0;
    else
        teams_box->parameters.text_box_parameters.string_list_index = 1;

    widget_instance *time_box = first_text_box(group->next->next->next->next);
    int time_limit = edit_playlist_profile->universal_variant.time_limit;
    if ( time_limit > 36000 )
    {
        switch ( time_limit )
        {
            case 45000: time_box->parameters.text_box_parameters.string_list_index = 4; break;
            case 54000: time_box->parameters.text_box_parameters.string_list_index = 5; break;
            case 81000: time_box->parameters.text_box_parameters.string_list_index = 6; break;
            default:    time_box->parameters.text_box_parameters.string_list_index = 0; break;
        }
    }
    else if ( time_limit == 36000 )
    {
        time_box->parameters.text_box_parameters.string_list_index = 3;
    }
    else if ( time_limit == 18000 )
    {
        time_box->parameters.text_box_parameters.string_list_index = 1;
    }
    else if ( time_limit == 27000 )
    {
        time_box->parameters.text_box_parameters.string_list_index = 2;
    }
    else
    {
        time_box->parameters.text_box_parameters.string_list_index = 0;
    }

    return 1;
}
