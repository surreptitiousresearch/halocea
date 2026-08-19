/* unit_animation_compute_interpolation_frame_count @ 0x836C82E0
   Chooses how many frames to interpolate over when transitioning between two
   animation states. */

#include <stdint.h>
#include "headers/unit_animation_state.h"

int unit_animation_compute_interpolation_frame_count(int16_t old_state, int16_t new_state)
{
    int result = 6;

    if ( (new_state == _unit_state_idle || new_state == _unit_state_turn_left || new_state == _unit_state_turn_right)
      && (old_state == _unit_state_idle || old_state == _unit_state_turn_left || old_state == _unit_state_turn_right) )
        result = 1;

    if ( new_state == _unit_state_land_hard || new_state == _unit_state_land_soft )
        return 2;
    return result;
}
