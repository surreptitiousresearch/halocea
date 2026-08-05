/* unit_animation_state_interruptable @0x836C7EF8 — can the unit's current animation state be replaced by
 * new_state? Locomotion/overlay states (move/stunned/slide/flying/airborne/land, melee_continuous,
 * feeding, leap_airborne) are always interruptable; turn/open/close yield only to a non-idle state; the
 * two death states yield only to each other; committed actions (impulse, melees, throw_grenade,
 * resurrects, leaps) yield only to a hard ping; hard_ping/seat_enter/seat_exit/custom never yield. */

#include <stdint.h>
#include "headers/unit_animation.h"
#include "headers/unit_animation_state.h"

uint8_t unit_animation_state_interruptable(unit_animation *animation, int16_t new_state)
{
    int state = (uint8_t)animation->state;
    switch ( state )
    {
        case _unit_state_turn_left: case _unit_state_turn_right: case _unit_state_opening: case _unit_state_closing:
            return new_state != _unit_state_idle;

        case _unit_state_move_front: case _unit_state_move_back: case _unit_state_move_left: case _unit_state_move_right: case _unit_state_stunned_move_front: case _unit_state_stunned_move_back: case _unit_state_stunned_move_left: case _unit_state_stunned_move_right: case _unit_state_slide_front: case _unit_state_slide_back:
        case _unit_state_slide_left: case _unit_state_slide_right: case _unit_state_flying_front: case _unit_state_flying_back: case _unit_state_flying_left: case _unit_state_flying_right: case _unit_state_airborne: case _unit_state_land_soft: case _unit_state_land_hard:
        case _unit_state_melee_continuous: case _unit_state_feeding: case _unit_state_leap_airborne:
            return 1;

        case _unit_state_dying_airborne: case _unit_state_dying:
            return new_state >= _unit_state_dying_airborne && new_state <= _unit_state_dying;

        case _unit_state_ai_impulse: case _unit_state_melee_attack: case _unit_state_melee_airborne: case _unit_state_throw_grenade: case _unit_state_resurrect_front: case _unit_state_resurrect_back: case _unit_state_leap_start: case _unit_state_leap_melee:
            return new_state == _unit_state_hard_ping;

        case _unit_state_hard_ping: case _unit_state_entering_seat: case _unit_state_exiting_seat: case _unit_state_user_animation:
            return 0;

        default:  /* states not listed (incl. 0,1) are interruptable */
            return 1;
    }
}
