/* playlist_profile_initialize_ctf_rules @0x836948B8 — UI init callback (sibling of
 * playlist_profile_initialize_racing_rules / _slayer_rules / _koth_rules): when the CTF rules screen is first
 * shown, seed its six sibling text-box selections from the edited game variant. Box 0 = assault on/off. Box 1
 * maps ctf.single_flag_time (1800/3600/5400/9000/18000 -> 1/2/3/4/5, else 0). Box 2 = flag_must_reset on/off.
 * Box 3 = flag_at_home_to_score on/off. Box 4 maps universal score_to_win (3/5/10/15 -> 1/2/3/4, else 0). Box 5
 * maps universal time_limit ticks (18000/27000/36000/45000/54000/81000 -> 1/2/3/4/5/6, else 0). Each text box
 * is the first widget_type==2 descendant of successive sibling groups under widget->children. Returns 1, or 0
 * if no profile is being edited. The nested single_flag_time / time_limit if-else ladders are written as the
 * switches they encode. */

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

uint8_t playlist_profile_initialize_ctf_rules(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    game_variant *edit_playlist_profile = player_ui_get_edit_playlist_profile();
    if ( !edit_playlist_profile )
        return 0;

    widget_instance *group = widget->children;

    widget_instance *assault_box = first_text_box(group);
    if ( edit_playlist_profile->game_engine_variant.ctf.assault )
        assault_box->parameters.text_box_parameters.string_list_index = 0;
    else
        assault_box->parameters.text_box_parameters.string_list_index = 1;

    widget_instance *single_flag_box = first_text_box(group->next);
    switch ( edit_playlist_profile->game_engine_variant.ctf.single_flag_time )
    {
        case 1800:  single_flag_box->parameters.text_box_parameters.string_list_index = 1; break;
        case 3600:  single_flag_box->parameters.text_box_parameters.string_list_index = 2; break;
        case 5400:  single_flag_box->parameters.text_box_parameters.string_list_index = 3; break;
        case 9000:  single_flag_box->parameters.text_box_parameters.string_list_index = 4; break;
        case 18000: single_flag_box->parameters.text_box_parameters.string_list_index = 5; break;
        default:    single_flag_box->parameters.text_box_parameters.string_list_index = 0; break;
    }

    widget_instance *flag_reset_box = first_text_box(group->next->next);
    if ( edit_playlist_profile->game_engine_variant.ctf.flag_must_reset )
        flag_reset_box->parameters.text_box_parameters.string_list_index = 0;
    else
        flag_reset_box->parameters.text_box_parameters.string_list_index = 1;

    widget_instance *at_home_box = first_text_box(group->next->next->next);
    if ( edit_playlist_profile->game_engine_variant.ctf.flag_at_home_to_score )
        at_home_box->parameters.text_box_parameters.string_list_index = 0;
    else
        at_home_box->parameters.text_box_parameters.string_list_index = 1;

    widget_instance *score_box = first_text_box(group->next->next->next->next);
    switch ( edit_playlist_profile->universal_variant.score_to_win )
    {
        case 3:  score_box->parameters.text_box_parameters.string_list_index = 1; break;
        case 5:  score_box->parameters.text_box_parameters.string_list_index = 2; break;
        case 10: score_box->parameters.text_box_parameters.string_list_index = 3; break;
        case 15: score_box->parameters.text_box_parameters.string_list_index = 4; break;
        default: score_box->parameters.text_box_parameters.string_list_index = 0; break;
    }

    widget_instance *time_box = first_text_box(group->next->next->next->next->next);
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
