/* player_control_action_test_check_reset_input_blob @0x836DF190 — fold one frame's input blob into the
 * action-test latch (action_test_flags) and reconcile the button-inhibit bookkeeping.
 *
 * First, a held Back/B (or the abstracted action button 8) skips a skippable cinematic when not in coop.
 * Then each relevant control in the input blob sets its corresponding action_test_flags bit: jump, crouch,
 * accept, back, primary trigger, melee, grenade, and the four facing/throttle directions (positive vs
 * negative axis -> distinct bits).
 *
 * Finally, for the action/accept/back buttons that are NOT currently being tested but ARE inhibited, the
 * inhibit bit is refreshed from the live button state and the raw control bit is masked out of the input so
 * it is not double-consumed this frame. The accept handling differs depending on whether controls are
 * swapped (the accept action maps to a different physical button). */

#include <stdint.h>
#include "headers/player_control_globals.h"
#include "headers/input_blob.h"
#include "headers/unit_control_flags.h"
#include "headers/player_control_flags.h"
#include "headers/player_control_action_test_flags.h"
#include "headers/control_button.h"
#include "headers/blam_data_globals.h"


extern uint8_t input_abstraction_is_action_button_down(int16_t local_player_index, int16_t button);
extern uint8_t cinematic_can_be_skipped(void);
extern void main_skip_cinematic(void);

void player_control_action_test_check_reset_input_blob(input_blob *input)
{
    if ( (input->back || input_abstraction_is_action_button_down(0, _button_start))
         && cinematic_can_be_skipped()
         && hcex_coop_local_player_index < 0 )
    {
        main_skip_cinematic();
    }

    player_control_globals_t *globals = player_control_globals;

    if ( (input->unit_control_flags & (1u << _unit_control_action_bit)) != 0 )
        globals->action_test_flags |= (1u << _player_control_action_test_action_bit);
    if ( (input->unit_control_flags & (1u << _unit_control_jump_bit)) != 0 )
        globals->action_test_flags |= (1u << _player_control_action_test_jump_bit);
    if ( input->accept )
        globals->action_test_flags |= (1u << _player_control_action_test_accept_bit);
    if ( input->back )
        globals->action_test_flags |= (1u << _player_control_action_test_back_bit);
    if ( input->primary_trigger > 0.0f )
        globals->action_test_flags |= (1u << _player_control_action_test_primary_trigger_bit);
    if ( (input->unit_control_flags & (1u << _unit_control_throw_grenade_bit)) != 0 )
        globals->action_test_flags |= (1u << _player_control_action_test_grenade_trigger_bit);
    if ( (input->player_control_flags & (1u << _player_control_zoom_bit)) != 0 )
        globals->action_test_flags |= (1u << _player_control_action_test_zoom_bit);

    float facing_pitch = input->facing_delta.n[1];
    if ( facing_pitch > 0.0f )
        globals->action_test_flags |= (1u << _player_control_action_test_look_positive_pitch_bit);
    else if ( facing_pitch < 0.0f )
        globals->action_test_flags |= (1u << _player_control_action_test_look_negative_pitch_bit);

    float facing_yaw = input->facing_delta.n[0];
    if ( facing_yaw > 0.0f )
        globals->action_test_flags |= (1u << _player_control_action_test_look_positive_yaw_bit);
    else if ( facing_yaw < 0.0f )
        globals->action_test_flags |= (1u << _player_control_action_test_look_negative_yaw_bit);

    float throttle_forward = input->throttle.n[0];
    if ( throttle_forward > 0.0f )
        globals->action_test_flags |= (1u << _player_control_action_test_move_forward_bit);
    else if ( throttle_forward < 0.0f )
        globals->action_test_flags |= (1u << _player_control_action_test_move_backward_bit);

    float throttle_strafe = input->throttle.n[1];
    if ( throttle_strafe > 0.0f )
        globals->action_test_flags |= (1u << _player_control_action_test_move_right_bit);
    else if ( throttle_strafe < 0.0f )
        globals->action_test_flags |= (1u << _player_control_action_test_move_left_bit);

    /* jump/action button (unit control 0x40): refresh inhibit + strip from raw input */
    if ( (globals->testing_for_action_flags & (1u << _player_control_action_test_action_bit)) == 0 )
    {
        unsigned int inhibit = globals->inhibit_button_flags;
        if ( (inhibit & (1u << _player_control_action_test_action_bit)) == 0 )
            goto strip_action;       /* not inhibited: leave the raw bit in place */
        if ( (input->unit_control_flags & (1u << _unit_control_action_bit)) != 0 )
            globals->inhibit_button_flags = inhibit | (1u << _player_control_action_test_action_bit);
        else
            globals->inhibit_button_flags = inhibit & ~(1u << _player_control_action_test_action_bit);
    }
    input->unit_control_flags &= ~(1u << _unit_control_action_bit);
strip_action:;

    int accept_being_tested = globals->testing_for_action_flags & (1u << _player_control_action_test_accept_bit);
    if ( !controls_swapped )
    {
        /* unswapped: accept maps to the action button (unit control 0x40) */
        if ( !accept_being_tested )
        {
            unsigned int inhibit = globals->inhibit_button_flags;
            if ( (inhibit & (1u << _player_control_action_test_accept_bit)) == 0 )
                goto check_back_unswapped;
            if ( (input->unit_control_flags & (1u << _unit_control_action_bit)) != 0 )
                globals->inhibit_button_flags = inhibit | (1u << _player_control_action_test_accept_bit);
            else
                globals->inhibit_button_flags = inhibit & ~(1u << _player_control_action_test_accept_bit);
        }
        input->unit_control_flags &= ~(1u << _unit_control_action_bit);
    check_back_unswapped:
        if ( (globals->testing_for_action_flags & (1u << _player_control_action_test_back_bit)) == 0 )
        {
            unsigned int inhibit = globals->inhibit_button_flags;
            if ( (inhibit & (1u << _player_control_action_test_back_bit)) == 0 )
                return;
            if ( (input->player_control_flags & (1u << _player_control_rotate_weapons_bit)) != 0 )
            {
                globals->inhibit_button_flags = inhibit | (1u << _player_control_action_test_back_bit);
                input->player_control_flags &= ~(1u << _player_control_rotate_weapons_bit);
                return;
            }
            globals->inhibit_button_flags = inhibit & ~(1u << _player_control_action_test_back_bit);
        }
        input->player_control_flags &= ~(1u << _player_control_rotate_weapons_bit);
    }
    else
    {
        /* swapped: accept maps to the crouch button (unit control 0x2) */
        if ( !accept_being_tested )
        {
            unsigned int inhibit = globals->inhibit_button_flags;
            if ( (inhibit & (1u << _player_control_action_test_accept_bit)) != 0 )
            {
                if ( (input->unit_control_flags & (1u << _unit_control_jump_bit)) != 0 )
                    globals->inhibit_button_flags = inhibit | (1u << _player_control_action_test_accept_bit);
                else
                    globals->inhibit_button_flags = inhibit & ~(1u << _player_control_action_test_accept_bit);
                input->unit_control_flags &= ~(1u << _unit_control_jump_bit);
            }
        }
        else
        {
            input->unit_control_flags &= ~(1u << _unit_control_jump_bit);
        }

        if ( (globals->testing_for_action_flags & (1u << _player_control_action_test_back_bit)) == 0 )
        {
            unsigned int inhibit = globals->inhibit_button_flags;
            /* swapped mode reuses the accept inhibit slot (bit 2) for the back action */
            if ( (inhibit & (1u << _player_control_action_test_accept_bit)) == 0 )
                return;
            if ( (input->player_control_flags & (1u << _player_control_rotate_grenades_bit)) != 0 )
                globals->inhibit_button_flags = inhibit | (1u << _player_control_action_test_accept_bit);
            else
                globals->inhibit_button_flags = inhibit & ~(1u << _player_control_action_test_accept_bit);
        }
        input->player_control_flags &= ~(1u << _player_control_rotate_grenades_bit);
    }
}
