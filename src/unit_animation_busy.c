/* unit_animation_busy @ 0x836C8008 — true when the unit's locomotion state is one of the "busy" states
 * that should block other actions (entering/exiting seats, throwing, various transitions). The explicit
 * non-busy cases (28, 36, 37, 38, 40) are listed in the original switch for clarity but simply fall
 * through to the default 0. */

#include <stdint.h>
#include "headers/unit_animation.h"
#include "headers/unit_animation_state.h"

uint8_t unit_animation_busy(unit_animation *animation)
{
    uint8_t result = 0;
    switch ( (unsigned __int8)animation->state )
    {
        case _unit_state_hard_ping: case _unit_state_dying_airborne: case _unit_state_dying:
        case _unit_state_entering_seat: case _unit_state_exiting_seat:
        case _unit_state_ai_impulse: case _unit_state_melee_attack: case _unit_state_melee_airborne:
        case _unit_state_melee_continuous: case _unit_state_throw_grenade:
        case _unit_state_resurrect_front: case _unit_state_resurrect_back:
        case _unit_state_leap_start: case _unit_state_leap_melee:
            result = 1;
            break;
        case _unit_state_user_animation: case _unit_state_feeding: case _unit_state_opening:
        case _unit_state_closing: case _unit_state_leap_airborne:
            return result;
    }
    return result;
}
