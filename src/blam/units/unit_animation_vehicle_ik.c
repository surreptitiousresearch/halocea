/* unit_animation_vehicle_ik @0x836C8178 — should vehicle-seat inverse-kinematics be applied for the
 * unit's current animation state? Returns 0 for the enter/exit/eject transition states that must not be
 * IK-driven, 1 otherwise. Pure predicate over unit_animation->state (DB-verified switch). */

#include <stdint.h>
#include "headers/unit_animation.h"
#include "headers/unit_animation_state.h"

int unit_animation_vehicle_ik(unit_animation *animation)
{
    int state = (uint8_t)animation->state;
    int result = 1;

    switch (state)
    {
        case _unit_state_hard_ping:
        case _unit_state_dying_airborne:
        case _unit_state_dying:
        case _unit_state_entering_seat:
        case _unit_state_exiting_seat:
        case _unit_state_ai_impulse:
        case _unit_state_resurrect_front:
        case _unit_state_resurrect_back:
            result = 0;
            break;
        case _unit_state_user_animation:
        case _unit_state_melee_attack:
        case _unit_state_melee_airborne:
        case _unit_state_melee_continuous:
        case _unit_state_throw_grenade:
            return result;
    }

    return result;
}
