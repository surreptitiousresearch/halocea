/* playlist_profile_change_racing_rules @0x83693D10 — UI callback: when the Racing rules screen is accepted, push
 * its five sibling text-box selections back into the profile's game variant, then pop the rules screen off the
 * widget stack. Box 1 maps ctf.single_flag_time (index 0/1/2+ -> 0/1/2 laps). Box 2 maps terminator.ignored
 * (0/1/2+ -> 0/1/2). Box 3 maps universal_variant.score_to_win (0/1/2/3/4/5 -> 1/3/5/10/15/25). Box 4 sets
 * universal_variant.teams (0 -> 1 team-race, 1 -> 0 free-for-all). Box 5 maps universal_variant.time_limit in
 * ticks (0/1/2/3/4/5/6+ -> 0/18000/27000/36000/45000/54000/81000). Each box is the first widget_type==2
 * descendant of successive sibling groups under the callback widget's grandparent. Returns 1, or 0 with no side
 * effects if no profile is being edited.
 *
 * Sibling of playlist_profile_change_ctf_rules. DEVIATION: the decompiler's per-case goto/fall-through around the
 * score and time boxes (case 0 assigns directly and jumps past a shared store) is folded into plain value-mapping
 * switches; semantics are identical. Unlike the ctf callback, the no-profile early-out returns 0 (per disasm). */

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

uint8_t playlist_profile_change_racing_rules(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    game_variant *edit_playlist_profile = player_ui_get_edit_playlist_profile();
    widget_instance *parent = widget->parent->parent;
    if ( !edit_playlist_profile )
        return 0;

    widget_instance *group = parent->children;

    unsigned int laps_index = (uint16_t)first_text_box(group)->parameters.text_box_parameters.string_list_index;
    if ( laps_index == 0 )
        edit_playlist_profile->game_engine_variant.ctf.single_flag_time = 0;
    else if ( laps_index == 1 )
        edit_playlist_profile->game_engine_variant.ctf.single_flag_time = 1;
    else if ( laps_index < 3 )
        edit_playlist_profile->game_engine_variant.ctf.single_flag_time = 2;

    unsigned int ignored_index =
            (uint16_t)first_text_box(group->next)->parameters.text_box_parameters.string_list_index;
    if ( ignored_index == 0 )
        edit_playlist_profile->game_engine_variant.terminator.ignored = 0;
    else if ( ignored_index == 1 )
        edit_playlist_profile->game_engine_variant.terminator.ignored = 1;
    else if ( ignored_index < 3 )
        edit_playlist_profile->game_engine_variant.terminator.ignored = 2;

    unsigned int score_index =
            (uint16_t)first_text_box(group->next->next)->parameters.text_box_parameters.string_list_index;
    if ( score_index <= 5 )
    {
        switch ( score_index )
        {
            case 1:  edit_playlist_profile->universal_variant.score_to_win = 3;  break;
            case 2:  edit_playlist_profile->universal_variant.score_to_win = 5;  break;
            case 3:  edit_playlist_profile->universal_variant.score_to_win = 10; break;
            case 4:  edit_playlist_profile->universal_variant.score_to_win = 15; break;
            case 0:  edit_playlist_profile->universal_variant.score_to_win = 1;  break;
            default: edit_playlist_profile->universal_variant.score_to_win = 25; break;   /* case 5 */
        }
    }

    unsigned int teams_index =
            (uint16_t)first_text_box(group->next->next->next)->parameters.text_box_parameters.string_list_index;
    if ( teams_index == 0 )
        edit_playlist_profile->universal_variant.teams = 1;
    else if ( teams_index == 1 )
        edit_playlist_profile->universal_variant.teams = 0;

    unsigned int time_index = (uint16_t)first_text_box(group->next->next->next->next)
            ->parameters.text_box_parameters.string_list_index;
    if ( time_index <= 6 )
    {
        switch ( time_index )
        {
            case 1:  edit_playlist_profile->universal_variant.time_limit = 18000; break;
            case 2:  edit_playlist_profile->universal_variant.time_limit = 27000; break;
            case 3:  edit_playlist_profile->universal_variant.time_limit = 36000; break;
            case 4:  edit_playlist_profile->universal_variant.time_limit = 45000; break;
            case 5:  edit_playlist_profile->universal_variant.time_limit = 54000; break;
            case 0:  edit_playlist_profile->universal_variant.time_limit = 0;     break;
            default: edit_playlist_profile->universal_variant.time_limit = 81000; break;   /* case 6 */
        }
    }

    ui_widgets_pop_stack(parent->local_player_index);
    return 1;
}
