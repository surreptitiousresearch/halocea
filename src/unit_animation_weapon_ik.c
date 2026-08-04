/* unit_animation_weapon_ik @0x836C811C — should the weapon (arm) inverse-kinematics be applied for the
 * unit's current animation state? True only when no action/overlay ping is playing and the state is not
 * one of the states that suppress weapon IK. Pure predicate over unit_animation. */

#include "headers/unit_animation.h"
#include "headers/unit_animation_state.h"

int unit_animation_weapon_ik(unit_animation *animation)
{
    unsigned int state = (unsigned __int8)animation->state;
    int result = animation->action == 0 && animation->soft_ping_animation.index == -1;

    /* suppressed for: every state from hard_ping through resurrect_back, the seat-driven
     * flying states, and the leap states (same bands as unit_postprocess_node_matrices) */
    if (state <= _unit_state_resurrect_back)
    {
        if (state < _unit_state_hard_ping
            && (state < _unit_state_flying_front || state > _unit_state_flying_right))
            return result;
        return 0;
    }

    if (state >= _unit_state_leap_start && state <= _unit_state_leap_melee)
        return 0;

    return result;
}
