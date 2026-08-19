/* unit_scripting_set_maximum_vitality @0x838005F0 — set a unit's maximum body and shield vitality via
 * object_initialize_vitality, unless the unit is already dead (object flags +0x106 bit 0x4).
 *
 * DEVIATION: the decompiler spilled the two real script arguments across a long phantom float parameter list;
 * reconstructed as (int, float body_vitality, float shield_vitality). */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/object_damage_flags.h"
#include "headers/blam_data_globals.h"

extern void object_initialize_vitality(int object_index, const float *custom_body_vitality, const float *custom_shield_vitality);

void unit_scripting_set_maximum_vitality(int unit_index, float body_vitality, float shield_vitality)
{
    if ( unit_index == -1 )
        return;
    object_datum *unit =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    if ( (unit->object.damage_flags & (1u << _object_dead_bit)) != 0 )
        return;
    object_initialize_vitality(unit_index, &body_vitality, &shield_vitality);
}
