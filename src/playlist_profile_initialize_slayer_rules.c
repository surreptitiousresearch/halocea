/* playlist_profile_initialize_slayer_rules @0x83694EE0 — UI init callback (sibling of
 * playlist_profile_initialize_koth_rules / _item_options): when the Slayer rules screen is first shown, seed its
 * six sibling text-box selections from the edited game variant. Box 1 = ctf assault flag (assault==1 -> 1, else
 * 0); box 2 = flag_must_reset (==0 -> 1, ==1 -> 0, other -> left unset); box 3 = reset_on_capture (==1 -> 1, else
 * 0); box 4 maps score_to_win (10/15/25/50 -> 1/2/3/4, else 0); box 5 = teams on/off; box 6 maps time_limit in
 * ticks (18000/27000/36000/45000/54000/81000 -> 1/2/3/4/5/6, anything else including 0 -> 0). Each text box is
 * the first widget_type==2 descendant of successive sibling groups. Returns 1, or 0 if no profile is being edited.
 *
 * DEVIATION: the decompiler's `x && x == 1` idioms are exactly `x == 1` (assault/reset_on_capture). box 2's
 * flag_must_reset case writes nothing when the value is neither 0 nor 1 — reproduced verbatim (the box keeps its
 * prior selection). Per-case goto tails folded into value-mapping switches, as in the koth sibling. */

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

uint8_t playlist_profile_initialize_slayer_rules(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    game_variant *edit_playlist_profile = player_ui_get_edit_playlist_profile();
    if ( !edit_playlist_profile )
        return 0;

    widget_instance *group = widget->children;

    widget_instance *assault_box = first_text_box(group);
    assault_box->parameters.text_box_parameters.string_list_index =
            edit_playlist_profile->game_engine_variant.ctf.assault == 1;

    widget_instance *reset_box = first_text_box(group->next);
    if ( edit_playlist_profile->game_engine_variant.ctf.flag_must_reset )
    {
        if ( edit_playlist_profile->game_engine_variant.ctf.flag_must_reset == 1 )
            reset_box->parameters.text_box_parameters.string_list_index = 0;
    }
    else
    {
        reset_box->parameters.text_box_parameters.string_list_index = 1;
    }

    widget_instance *capture_box = first_text_box(group->next->next);
    capture_box->parameters.text_box_parameters.string_list_index =
            edit_playlist_profile->game_engine_variant.ctf.reset_on_capture == 1;

    widget_instance *score_box = first_text_box(group->next->next->next);
    switch ( edit_playlist_profile->universal_variant.score_to_win )
    {
        case 10: score_box->parameters.text_box_parameters.string_list_index = 1; break;
        case 15: score_box->parameters.text_box_parameters.string_list_index = 2; break;
        case 25: score_box->parameters.text_box_parameters.string_list_index = 3; break;
        case 50: score_box->parameters.text_box_parameters.string_list_index = 4; break;
        default: score_box->parameters.text_box_parameters.string_list_index = 0; break;
    }

    widget_instance *teams_box = first_text_box(group->next->next->next->next);
    if ( edit_playlist_profile->universal_variant.teams )
        teams_box->parameters.text_box_parameters.string_list_index = 0;
    else
        teams_box->parameters.text_box_parameters.string_list_index = 1;

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
