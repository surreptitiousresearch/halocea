/* unit_animation_overlay_action_loops @0x836C8088 — true when the unit's locomotion state is one of the two
 * looping states in the [turn_right, move_front] range (unit_animation.state, offset 0xB per DB). */

#include "headers/unit_animation.h"
#include "headers/unit_animation_state.h"

int unit_animation_overlay_action_loops(unit_animation *animation)
{
    unsigned int state = (unsigned char)animation->state;
    if (state >= _unit_state_turn_right)
        return state <= _unit_state_move_front;
    return 0;
}
