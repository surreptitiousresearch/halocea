/* playlist_profile_change_ctf_rules @0x83692FD0 — UI callback: when the CTF rules screen is accepted, push its
 * six sibling text-box selections back into the profile's game variant, then pop the rules screen off the widget
 * stack. Box 1 sets ctf.assault (index 0 -> assault, 1 -> plain). Box 2 maps to ctf.single_flag_time in ticks
 * (0/1/2/3/4+ -> 0/1800/3600/5400/9000/18000). Box 3 sets ctf.flag_must_reset (0 -> 1, 1 -> 0). Box 4 sets
 * ctf.flag_at_home_to_score (0 -> 1, 1 -> 0). Box 5 maps universal_variant.score_to_win (0/1/2/3/4+ ->
 * 1/3/5/10/15). Box 6 maps universal_variant.time_limit in ticks (0/1/2/3/4/5/6+ ->
 * 0/18000/27000/36000/45000/54000/81000). Each box is the first widget_type==2 descendant of successive sibling
 * groups under the callback widget's grandparent. Returns 1, or 0 with no side effects if no profile is being
 * edited.
 *
 * DEVIATION: the decompiler's per-case goto/fall-through around the score/time boxes (case 0 assigns directly and
 * jumps past a shared store) is folded into plain switches that assign a mapped value; semantics are identical. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/game_variant.h"
#include "headers/ui_widget_type.h"

extern game_variant * player_ui_get_edit_playlist_profile(void);
extern void ui_widgets_pop_stack(int16_t local_player_index);

static widget_instance *first_text_box(widget_instance *group)
{
    for ( widget_instance *child = group->children; child; child = child->next )
    {
        if ( child->widget_type == _ui_widget_type_spinner_list )
            return child;
    }
    return nullptr;
}

uint8_t playlist_profile_change_ctf_rules(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    game_variant *edit_playlist_profile = player_ui_get_edit_playlist_profile();
    widget_instance *parent = widget->parent->parent;
    if ( !edit_playlist_profile )
        return 1;

    widget_instance *group = parent->children;

    int assault_index = (unsigned __int16)first_text_box(group)->parameters.text_box_parameters.string_list_index;
    if ( assault_index == 0 )
        edit_playlist_profile->game_engine_variant.ctf.assault = 1;
    else if ( assault_index == 1 )
        edit_playlist_profile->game_engine_variant.ctf.assault = 0;

    unsigned int flag_time_index =
            (unsigned __int16)first_text_box(group->next)->parameters.text_box_parameters.string_list_index;
    if ( flag_time_index <= 5 )
    {
        switch ( flag_time_index )
        {
            case 0: edit_playlist_profile->game_engine_variant.ctf.single_flag_time = 0;    break;
            case 1: edit_playlist_profile->game_engine_variant.ctf.single_flag_time = 1800; break;
            case 2: edit_playlist_profile->game_engine_variant.ctf.single_flag_time = 3600; break;
            case 3: edit_playlist_profile->game_engine_variant.ctf.single_flag_time = 5400; break;
            case 4: edit_playlist_profile->game_engine_variant.ctf.single_flag_time = 9000; break;
            default: edit_playlist_profile->game_engine_variant.ctf.single_flag_time = 18000; break;
        }
    }

    widget_instance *reset_group = group->next->next;
    int reset_index = (unsigned __int16)first_text_box(reset_group)->parameters.text_box_parameters.string_list_index;
    if ( reset_index == 0 )
        edit_playlist_profile->game_engine_variant.ctf.flag_must_reset = 1;
    else if ( reset_index == 1 )
        edit_playlist_profile->game_engine_variant.ctf.flag_must_reset = 0;

    widget_instance *at_home_group = reset_group->next;
    int at_home_index =
            (unsigned __int16)first_text_box(at_home_group)->parameters.text_box_parameters.string_list_index;
    if ( at_home_index == 0 )
        edit_playlist_profile->game_engine_variant.ctf.flag_at_home_to_score = 1;
    else if ( at_home_index == 1 )
        edit_playlist_profile->game_engine_variant.ctf.flag_at_home_to_score = 0;

    widget_instance *score_group = at_home_group->next;
    unsigned int score_index =
            (unsigned __int16)first_text_box(score_group)->parameters.text_box_parameters.string_list_index;
    if ( score_index <= 4 )
    {
        switch ( score_index )
        {
            case 0: edit_playlist_profile->universal_variant.score_to_win = 1;  break;
            case 1: edit_playlist_profile->universal_variant.score_to_win = 3;  break;
            case 2: edit_playlist_profile->universal_variant.score_to_win = 5;  break;
            case 3: edit_playlist_profile->universal_variant.score_to_win = 10; break;
            default: edit_playlist_profile->universal_variant.score_to_win = 15; break;
        }
    }

    unsigned int time_index =
            (unsigned __int16)first_text_box(score_group->next)->parameters.text_box_parameters.string_list_index;
    if ( time_index <= 6 )
    {
        switch ( time_index )
        {
            case 0: edit_playlist_profile->universal_variant.time_limit = 0;     break;
            case 1: edit_playlist_profile->universal_variant.time_limit = 18000; break;
            case 2: edit_playlist_profile->universal_variant.time_limit = 27000; break;
            case 3: edit_playlist_profile->universal_variant.time_limit = 36000; break;
            case 4: edit_playlist_profile->universal_variant.time_limit = 45000; break;
            case 5: edit_playlist_profile->universal_variant.time_limit = 54000; break;
            default: edit_playlist_profile->universal_variant.time_limit = 81000; break;
        }
    }

    ui_widgets_pop_stack(parent->local_player_index);
    return 1;
}
