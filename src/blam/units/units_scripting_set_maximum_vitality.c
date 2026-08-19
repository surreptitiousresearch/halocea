/* units_scripting_set_maximum_vitality @0x83800660 — set maximum body and shield vitality on every unit in an
 * object list (skipping dead units), via object_initialize_vitality. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/object_damage_flags.h"
#include "headers/blam_data_globals.h"

extern int object_list_get_first(int object_list_index, int *reference_index);
extern int object_list_get_next(int object_list_index, int *reference_index);
extern void object_initialize_vitality(int object_index, const float *custom_body_vitality, const float *custom_shield_vitality);

void units_scripting_set_maximum_vitality(int object_list_index, float body_vitality, float shield_vitality)
{
    int iterator[4];
    for ( int object_index = object_list_get_first(object_list_index, iterator);
          object_index != -1;
          object_index = object_list_get_next(object_list_index, iterator) )
    {
        float shield = shield_vitality;
        float body = body_vitality;
        object_datum *unit = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
        if ( (unit->object.damage_flags & (1u << _object_dead_bit)) == 0 )
            object_initialize_vitality(object_index, &body, &shield);
    }
}
