/* unit_scripting_suspended @0x836C9D20 — suspend or resume a unit's physics: toggle the "suspended" flag
 * (unit flags +0x204 bit 0x1000000), zero its linear velocity (+0x68), and for bipeds (object type 0) clear
 * the airborne/physics flag (+0x4CC bit 0x1). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/biped_datum.h"
#include "headers/biped_datum_flags.h"
#include "headers/real_vector3d.h"
#include "headers/unit_flags.h"
#include "headers/blam_data_globals.h"


void unit_scripting_suspended(int unit_index, uint8_t suspend)
{
    if ( unit_index == -1 )
        return;
    unit_datum *unit =
        (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    unsigned int flags = unit->unit.flags | (1u << _unit_suspended_bit);
    if ( !suspend )
        flags = unit->unit.flags & ~(1u << _unit_suspended_bit);
    unit->unit.flags = flags;

    int16_t object_type = unit->object.type;
    unit->object.translational_velocity = *global_zero_vector3d;
    if ( !object_type )
    {
        biped_datum *biped =
            (biped_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
        biped->biped.flags &= ~(1u << _biped_airborne_bit);
    }
}
