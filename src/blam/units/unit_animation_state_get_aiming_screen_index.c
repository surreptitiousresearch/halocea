/* unit_animation_state_get_aiming_screen_index @0x836C8210 — map a unit animation state to the aiming
 * "screen" (weapon-class aiming animation) index used by the first-person overlay: idle/turn/flying/
 * land/open/close states -> aim_still (25), movement states -> aim_move (26), anything else -> -1.
 * The return value indexes weapon_class->animations (see unit_animation_set_state). */

#include <stdint.h>
#include "headers/unit_animation_state.h"
#include "headers/weapon_class_animation.h"

int16_t unit_animation_state_get_aiming_screen_index(int16_t state)
{
    if ( state > _unit_state_slide_right )
    {
        if ( state <= _unit_state_closing
             && (state >= _unit_state_opening
                 || (state >= _unit_state_flying_front && state <= _unit_state_land_hard)) )
            return _weapon_class_animation_aim_still;
        return -1;
    }
    if ( state >= _unit_state_move_front )
        return _weapon_class_animation_aim_move;
    if ( state == _unit_state_idle || state > _unit_state_gesture )
        return _weapon_class_animation_aim_still;
    return -1;
}
