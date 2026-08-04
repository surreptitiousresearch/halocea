/* unit_stop_custom_animation @0x836D0DC8 — stop a unit's scripted custom animation by returning its animation
 * state to idle (state 0), but only if it is currently in the custom-animation state (28). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/unit_animation_state.h"
#include "headers/blam_data_globals.h"

extern uint8_t unit_animation_set_state(int unit_index, int16_t new_state);

void unit_stop_custom_animation(int unit_index)
{
    if ( unit_index == -1 )
        return;
    unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    if ( unit->unit.animation.state == _unit_state_user_animation )
        unit_animation_set_state(unit_index, _unit_state_idle);
}
