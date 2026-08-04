/* unit_animation_state_loops @0x836C80A8 — does the given unit animation state loop (as opposed to
 * playing once)? Pure predicate over unit_animation->state. The one-shot (non-looping) states are the
 * turn/hard-ping/seat-transition/committed-action states enumerated below; every other state loops.
 *
 * Deviation: the decompiler emitted this as nested range comparisons over raw state bounds
 * (0x23/0x21/0x17/0x1A/0x1F ...); reproduced here as an equivalent switch over the named states
 * (verified state-by-state against the original branch structure). */

#include "headers/unit_animation.h"
#include "headers/unit_animation_state.h"

int unit_animation_state_loops(unit_animation *animation)
{
    unsigned int state = (unsigned __int8)animation->state;

    switch ( state )
    {
        case _unit_state_gesture:          /* 1  */
        case _unit_state_turn_left:        /* 2  */
        case _unit_state_turn_right:       /* 3  */
        case _unit_state_hard_ping:        /* 23 */
        case _unit_state_entering_seat:    /* 26 */
        case _unit_state_exiting_seat:     /* 27 */
        case _unit_state_user_animation:   /* 28 */
        case _unit_state_ai_impulse:       /* 29 */
        case _unit_state_melee_attack:     /* 30 */
        case _unit_state_melee_airborne:   /* 31 */
        case _unit_state_throw_grenade:    /* 33 */
        case _unit_state_resurrect_front:  /* 34 */
        case _unit_state_resurrect_back:   /* 35 */
        case _unit_state_leap_start:       /* 39 */
        case _unit_state_leap_melee:       /* 41 */
            return 0;
        default:
            return 1;
    }
}
