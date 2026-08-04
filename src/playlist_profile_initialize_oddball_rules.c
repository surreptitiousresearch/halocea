/* playlist_profile_initialize_oddball_rules @0x836951E8 — UI init callback (sibling of
 * playlist_profile_initialize_ctf_rules / _koth_rules / _slayer_rules): when the oddball rules screen is first
 * shown, seed its nine sibling text-box selections from the edited game variant. Each text box is the first
 * widget_type==2 descendant of successive sibling groups under widget->children. Returns 1, or 0 if no profile
 * is being edited.
 *
 * Box 0 = oddball.trait_with_ball    (1/2/3 -> 1/2/3, else 0)
 * Box 1 = oddball.trait_without_ball (1/2/3 -> 1/2/3, else 0)
 * Box 2 = oddball.speed_with_ball (slow -> 1, faster -> 2, normal/else -> 0; the decompiler read this
 *         through the ctf.single_flag_time union view — same physical bytes, retyped to the oddball arm)
 * Box 3 = oddball.oddball_ball_type  (1 -> 1, 2 -> 2, else 0)
 * Box 4 = ctf.assault union view     (on -> 0, off -> 1)
 * Box 5 = oddball.ball_spawn_count   (1..16 -> value-1, else 0)
 * Box 6 = universal score_to_win     (2/5/10/15 -> 1/2/3/4, else 0)
 * Box 7 = universal teams            (on -> 0, off -> 1)
 * Box 8 = universal time_limit ticks (18000/27000/36000/45000/54000/81000 -> 1/2/3/4/5/6, else 0)
 *
 * The single_flag_time / oddball_ball_type / time_limit if-else ladders are written as the switches they
 * encode. Boxes 2 and 4 read the ctf view of the engine-variant union at the offsets the decompiler resolved
 * (game_engine_variant is a union; these are the same physical bytes). */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/game_variant.h"
#include "headers/game_engine_trait.h"
#include "headers/oddball_ball_type.h"
#include "headers/oddball_speed.h"
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

uint8_t playlist_profile_initialize_oddball_rules(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    game_variant *edit_playlist_profile = player_ui_get_edit_playlist_profile();
    if ( !edit_playlist_profile )
        return 0;

    widget_instance *group = widget->children;

    widget_instance *trait_with_ball_box = first_text_box(group);
    switch ( edit_playlist_profile->game_engine_variant.oddball.trait_with_ball )
    {
        case _game_trait_invisible:        trait_with_ball_box->parameters.text_box_parameters.string_list_index = 1; break;
        case _game_trait_extra_damage:     trait_with_ball_box->parameters.text_box_parameters.string_list_index = 2; break;
        case _game_trait_damage_resistant: trait_with_ball_box->parameters.text_box_parameters.string_list_index = 3; break;
        default: trait_with_ball_box->parameters.text_box_parameters.string_list_index = 0; break;
    }

    widget_instance *trait_without_ball_box = first_text_box(group->next);
    switch ( edit_playlist_profile->game_engine_variant.oddball.trait_without_ball )
    {
        case _game_trait_invisible:        trait_without_ball_box->parameters.text_box_parameters.string_list_index = 1; break;
        case _game_trait_extra_damage:     trait_without_ball_box->parameters.text_box_parameters.string_list_index = 2; break;
        case _game_trait_damage_resistant: trait_without_ball_box->parameters.text_box_parameters.string_list_index = 3; break;
        default: trait_without_ball_box->parameters.text_box_parameters.string_list_index = 0; break;
    }

    /* deviation: decompiler read ctf.single_flag_time; the union arm in play is oddball.speed_with_ball
     * (same bytes at 0x4). */
    widget_instance *ball_speed_box = first_text_box(group->next->next);
    switch ( edit_playlist_profile->game_engine_variant.oddball.speed_with_ball )
    {
        case _oddball_speed_slow:   ball_speed_box->parameters.text_box_parameters.string_list_index = 1; break;
        case _oddball_speed_faster: ball_speed_box->parameters.text_box_parameters.string_list_index = 2; break;
        default:                    ball_speed_box->parameters.text_box_parameters.string_list_index = 0; break; /* _oddball_speed_normal */
    }

    widget_instance *ball_type_box = first_text_box(group->next->next->next);
    switch ( edit_playlist_profile->game_engine_variant.oddball.oddball_ball_type )
    {
        case _oddball_magic:      ball_type_box->parameters.text_box_parameters.string_list_index = 1; break;
        case _oddball_terminator: ball_type_box->parameters.text_box_parameters.string_list_index = 2; break;
        default: ball_type_box->parameters.text_box_parameters.string_list_index = 0; break;
    }

    widget_instance *assault_box = first_text_box(group->next->next->next->next);
    if ( edit_playlist_profile->game_engine_variant.ctf.assault )
        assault_box->parameters.text_box_parameters.string_list_index = 0;
    else
        assault_box->parameters.text_box_parameters.string_list_index = 1;

    widget_instance *ball_spawn_count_box = first_text_box(group->next->next->next->next->next);
    int ball_spawn_count = edit_playlist_profile->game_engine_variant.oddball.ball_spawn_count;
    if ( ball_spawn_count <= 0 || ball_spawn_count > 16 )
        ball_spawn_count_box->parameters.text_box_parameters.string_list_index = 0;
    else
        ball_spawn_count_box->parameters.text_box_parameters.string_list_index = ball_spawn_count - 1;

    widget_instance *score_box = first_text_box(group->next->next->next->next->next->next);
    switch ( edit_playlist_profile->universal_variant.score_to_win )
    {
        case 2:  score_box->parameters.text_box_parameters.string_list_index = 1; break;
        case 5:  score_box->parameters.text_box_parameters.string_list_index = 2; break;
        case 10: score_box->parameters.text_box_parameters.string_list_index = 3; break;
        case 15: score_box->parameters.text_box_parameters.string_list_index = 4; break;
        default: score_box->parameters.text_box_parameters.string_list_index = 0; break;
    }

    widget_instance *teams_box = first_text_box(group->next->next->next->next->next->next->next);
    if ( edit_playlist_profile->universal_variant.teams )
        teams_box->parameters.text_box_parameters.string_list_index = 0;
    else
        teams_box->parameters.text_box_parameters.string_list_index = 1;

    widget_instance *time_box = first_text_box(group->next->next->next->next->next->next->next->next);
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
