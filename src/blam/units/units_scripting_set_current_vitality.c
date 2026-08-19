/* units_scripting_set_current_vitality @0x83800840 — set current body and shield vitality on every unit in an
 * object list; see unit_scripting_set_current_vitality for the per-unit conversion and depletion logic. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/object_damage_flags.h"
#include "headers/blam_data_globals.h"

extern int object_list_get_first(int object_list_index, int *reference_index);
extern int object_list_get_next(int object_list_index, int *reference_index);
extern void object_deplete_shield(int object_index);
extern void object_deplete_body(int object_index);

void units_scripting_set_current_vitality(int object_list_index, float body_vitality, float shield_vitality)
{
    int iterator[14];
    for ( int object_index = object_list_get_first(object_list_index, iterator);
          object_index != -1;
          object_index = object_list_get_next(object_list_index, iterator) )
    {
        object_datum *unit = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
        if ( (unit->object.damage_flags & (1u << _object_dead_bit)) != 0 )
            continue;

        float maximum_shield = unit->object.maximum_shield_vitality;
        float shield_fraction;
        if ( maximum_shield > 0.0f )
            shield_fraction = shield_vitality < maximum_shield ? shield_vitality / unit->object.maximum_shield_vitality : 1.0f;
        else
            shield_fraction = 0.0f;

        float maximum_body = unit->object.maximum_body_vitality;
        float body_fraction;
        if ( maximum_body > 0.0f )
            body_fraction = body_vitality < maximum_body ? body_vitality / unit->object.maximum_body_vitality : 1.0f;
        else
            body_fraction = 0.0f;

        if ( unit->object.shield_vitality > 0.0f && shield_fraction <= 0.0f )
            object_deplete_shield(object_index);
        float previous_body_fraction = unit->object.body_vitality;
        unit->object.shield_vitality = shield_fraction;
        if ( previous_body_fraction > 0.0f && body_fraction <= 0.0f )
            object_deplete_body(object_index);
        unit->object.body_vitality = body_fraction;
    }
}
