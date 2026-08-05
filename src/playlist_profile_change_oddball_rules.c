/* playlist_profile_change_oddball_rules @0x83693898 — UI callback (sibling of
 * playlist_profile_change_ctf_rules / _koth_rules / _slayer_rules): when the oddball rules screen is accepted,
 * push its nine sibling text-box selections back into the edited game variant, then pop the rules screen off the
 * widget stack. Each box is the first widget_type==2 descendant of successive sibling groups under the callback
 * widget's grandparent. Returns 1, or 0 with no side effects if no profile is being edited.
 *
 * Box 0 = oddball.trait_with_ball    (index 0/1/2/3 -> none/invisible/extra_damage/damage_resistant)
 * Box 1 = oddball.trait_without_ball (index 0/1/2/3 -> none/invisible/extra_damage/damage_resistant)
 * Box 2 = oddball.speed_with_ball    (index 0/1/2 -> normal/slow/faster, else unchanged; the decompiler wrote
 *         this through the ctf.single_flag_time union view — same physical bytes, retyped to the oddball arm)
 * Box 3 = oddball.oddball_ball_type  (index 0/1/2 -> normal/magic/terminator, else unchanged)
 * Box 4 = ctf.assault view           (index 0 -> 1, 1 -> 0, else unchanged)
 * Box 5 = oddball.ball_spawn_count   (index 0..15 -> index+1)
 * Box 6 = universal score_to_win     (index 0/1/2/3/4 -> 1/2/5/10/15)
 * Box 7 = universal teams            (index 0 -> 1, 1 -> 0, else unchanged)
 * Box 8 = universal time_limit ticks (index 0/1/2/3/4/5/6 -> 0/18000/27000/36000/45000/54000/81000)
 *
 * Boxes 2 and 4 write the ctf view of the engine-variant union; these are the same physical bytes as the oddball
 * view for those fields. The score/time if-else-goto ladders the decompiler emitted are folded into the switches
 * they encode; semantics are identical. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/game_variant.h"
#include "headers/game_engine_trait.h"
#include "headers/oddball_ball_type.h"
#include "headers/oddball_speed.h"
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

uint8_t playlist_profile_change_oddball_rules(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    game_variant *edit_playlist_profile = player_ui_get_edit_playlist_profile();
    widget_instance *parent = widget->parent->parent;
    if ( !edit_playlist_profile )
        return 0;

    widget_instance *group = parent->children;

    unsigned int trait_with_ball_index =
            (uint16_t)first_text_box(group)->parameters.text_box_parameters.string_list_index;
    if ( trait_with_ball_index <= 3 )
    {
        switch ( trait_with_ball_index )
        {
            case 1:  edit_playlist_profile->game_engine_variant.oddball.trait_with_ball = _game_trait_invisible;        break;
            case 2:  edit_playlist_profile->game_engine_variant.oddball.trait_with_ball = _game_trait_extra_damage;     break;
            case 3:  edit_playlist_profile->game_engine_variant.oddball.trait_with_ball = _game_trait_damage_resistant; break;
            default: edit_playlist_profile->game_engine_variant.oddball.trait_with_ball = _game_trait_none;             break;
        }
    }

    widget_instance *trait_without_ball_group = group->next;
    unsigned int trait_without_ball_index =
            (uint16_t)first_text_box(trait_without_ball_group)->parameters.text_box_parameters.string_list_index;
    if ( trait_without_ball_index <= 3 )
    {
        switch ( trait_without_ball_index )
        {
            case 1:  edit_playlist_profile->game_engine_variant.oddball.trait_without_ball = _game_trait_invisible;        break;
            case 2:  edit_playlist_profile->game_engine_variant.oddball.trait_without_ball = _game_trait_extra_damage;     break;
            case 3:  edit_playlist_profile->game_engine_variant.oddball.trait_without_ball = _game_trait_damage_resistant; break;
            default: edit_playlist_profile->game_engine_variant.oddball.trait_without_ball = _game_trait_none;             break;
        }
    }

    widget_instance *single_flag_group = trait_without_ball_group->next;
    unsigned int single_flag_index =
            (uint16_t)first_text_box(single_flag_group)->parameters.text_box_parameters.string_list_index;
    /* deviation: decompiler wrote ctf.single_flag_time; the union arm in play is oddball.speed_with_ball
     * (same bytes at 0x4). */
    if ( single_flag_index == 0 )
        edit_playlist_profile->game_engine_variant.oddball.speed_with_ball = _oddball_speed_normal;
    else if ( single_flag_index == 1 )
        edit_playlist_profile->game_engine_variant.oddball.speed_with_ball = _oddball_speed_slow;
    else if ( single_flag_index < 3 )
        edit_playlist_profile->game_engine_variant.oddball.speed_with_ball = _oddball_speed_faster;

    widget_instance *ball_type_group = single_flag_group->next;
    unsigned int ball_type_index =
            (uint16_t)first_text_box(ball_type_group)->parameters.text_box_parameters.string_list_index;
    if ( ball_type_index == 0 )
        edit_playlist_profile->game_engine_variant.oddball.oddball_ball_type = _oddball_normal;
    else if ( ball_type_index == 1 )
        edit_playlist_profile->game_engine_variant.oddball.oddball_ball_type = _oddball_magic;
    else if ( ball_type_index < 3 )
        edit_playlist_profile->game_engine_variant.oddball.oddball_ball_type = _oddball_terminator;

    widget_instance *assault_group = ball_type_group->next;
    unsigned int assault_index =
            (uint16_t)first_text_box(assault_group)->parameters.text_box_parameters.string_list_index;
    if ( assault_index == 0 )
        edit_playlist_profile->game_engine_variant.ctf.assault = 1;
    else if ( assault_index == 1 )
        edit_playlist_profile->game_engine_variant.ctf.assault = 0;

    widget_instance *ball_spawn_count_group = assault_group->next;
    unsigned int ball_spawn_count_index =
            (uint16_t)first_text_box(ball_spawn_count_group)->parameters.text_box_parameters.string_list_index;
    if ( ball_spawn_count_index < 16 )
        edit_playlist_profile->game_engine_variant.oddball.ball_spawn_count = (int16_t)ball_spawn_count_index + 1;

    widget_instance *score_group = ball_spawn_count_group->next;
    unsigned int score_index =
            (uint16_t)first_text_box(score_group)->parameters.text_box_parameters.string_list_index;
    if ( score_index <= 4 )
    {
        switch ( score_index )
        {
            case 0:  edit_playlist_profile->universal_variant.score_to_win = 1;  break;
            case 1:  edit_playlist_profile->universal_variant.score_to_win = 2;  break;
            case 2:  edit_playlist_profile->universal_variant.score_to_win = 5;  break;
            case 3:  edit_playlist_profile->universal_variant.score_to_win = 10; break;
            default: edit_playlist_profile->universal_variant.score_to_win = 15; break;
        }
    }

    widget_instance *teams_group = score_group->next;
    unsigned int teams_index =
            (uint16_t)first_text_box(teams_group)->parameters.text_box_parameters.string_list_index;
    if ( teams_index == 0 )
        edit_playlist_profile->universal_variant.teams = 1;
    else if ( teams_index == 1 )
        edit_playlist_profile->universal_variant.teams = 0;

    widget_instance *time_group = teams_group->next;
    unsigned int time_index =
            (uint16_t)first_text_box(time_group)->parameters.text_box_parameters.string_list_index;
    if ( time_index <= 6 )
    {
        switch ( time_index )
        {
            case 0:  edit_playlist_profile->universal_variant.time_limit = 0;     break;
            case 1:  edit_playlist_profile->universal_variant.time_limit = 18000; break;
            case 2:  edit_playlist_profile->universal_variant.time_limit = 27000; break;
            case 3:  edit_playlist_profile->universal_variant.time_limit = 36000; break;
            case 4:  edit_playlist_profile->universal_variant.time_limit = 45000; break;
            case 5:  edit_playlist_profile->universal_variant.time_limit = 54000; break;
            default: edit_playlist_profile->universal_variant.time_limit = 81000; break;
        }
    }

    ui_widgets_pop_stack(parent->local_player_index);
    return 1;
}
