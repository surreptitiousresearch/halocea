/* unit_scripting_set_current_vitality @0x83800720 — set a unit's current body and shield vitality. The script
 * values are absolute amounts; they are converted to fractions of the unit's maximums (max body +0xD8, max
 * shield +0xDC) and stored as body fraction +0xE0 and shield fraction +0xE4. Crossing to zero from a positive
 * value depletes the corresponding pool. Dead units (object flags +0x106 bit 0x4) are ignored. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/object_damage_flags.h"
#include "headers/blam_data_globals.h"

extern void object_deplete_shield(int object_index);
extern void object_deplete_body(int object_index);

void unit_scripting_set_current_vitality(int unit_index, float body_vitality, float shield_vitality)
{
    if ( unit_index == -1 )
        return;
    unit_datum *unit =
        (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    if ( (unit->object.damage_flags & (1u << _object_dead_bit)) != 0 )
        return;

    float maximum_shield = unit->object.maximum_shield_vitality;
    float shield_fraction;
    if ( maximum_shield > 0.0f )
        shield_fraction = shield_vitality < maximum_shield ? shield_vitality / unit->object.maximum_shield_vitality : 1.0f;
    else
        shield_fraction = 0.0f;

    float maximum_body = unit->object.maximum_body_vitality;
    float body_fraction = 1.0f;
    if ( maximum_body > 0.0f )
    {
        if ( body_vitality < maximum_body )
            body_fraction = body_vitality / unit->object.maximum_body_vitality;
    }
    else
    {
        body_fraction = 0.0f;
    }

    if ( unit->object.shield_vitality > 0.0f && shield_fraction <= 0.0f )
        object_deplete_shield(unit_index);
    float previous_body_fraction = unit->object.body_vitality;
    unit->object.shield_vitality = shield_fraction;
    if ( previous_body_fraction > 0.0f && body_fraction <= 0.0f )
        object_deplete_body(unit_index);
    unit->object.body_vitality = body_fraction;
}
