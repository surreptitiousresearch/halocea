/* unit_is_playing_custom_animation @0x836CCF68 — return whether a unit is currently in the scripted
 * custom-animation state (animation state +0x2A3 == 28). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/unit_animation_state.h"
#include "headers/blam_data_globals.h"


uint8_t unit_is_playing_custom_animation(int unit_index)
{
    if ( unit_index == -1 )
        return 0;
    unit_datum *object_data =
        (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    return (unsigned char)object_data->unit.animation.state == _unit_state_user_animation;
}
