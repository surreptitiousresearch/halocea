/* unit_leap_begin @0x836CEEC0 — start a unit's leap (e.g. flood/hunter pounce). Fails if the unit's
 * animation is busy (animation block @ object +664), or if the unit is grounded with the leap-inhibit flag
 * clear path blocked (state word @ +180 zero and flags bit0 @ +1228 set), or if the leap animation state
 * (39) can't be entered. On success, optionally aligns the unit's facing to alignment_vector. Returns 1 on
 * a started leap, 0 otherwise. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_animation.h"
#include "headers/real_vector2d.h"
#include "headers/unit_animation_state.h"
#include "headers/biped_datum.h"
#include "headers/biped_datum_flags.h"
#include "headers/blam_data_globals.h"

extern uint8_t unit_animation_busy(unit_animation *animation);
extern uint8_t unit_animation_set_state(int unit_index, int16_t new_state);
extern void unit_align_facing(int unit_index, const real_vector2d *alignment_vector);

uint8_t unit_leap_begin(int unit_index, const real_vector2d *alignment_vector)
{
    biped_datum *object = (biped_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    if ( unit_animation_busy(&object->unit.animation) )
        return 0;

    unsigned char blocked = 0;
    if ( !object->object.type )
        blocked = object->biped.flags & (1u << _biped_airborne_bit);
    if ( blocked || !unit_animation_set_state(unit_index, _unit_state_leap_start) )
        return 0;

    if ( alignment_vector )
        unit_align_facing(unit_index, alignment_vector);
    return 1;
}
