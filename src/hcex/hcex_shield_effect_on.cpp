/* hcex_shield_effect_on @0x83681CA0 — return an object's shield-effect fraction (current_shield_damage), or 0
 * when the object's type is nonzero (i.e. not the biped type the shield effect applies to). Resolves the
 * object datum via the object-header data-array. */

#include "../headers/data_array.h"
#include "../headers/object_header_datum.h"

extern data_array *object_header_data;

extern "C" float hcex_shield_effect_on(int id)
{
    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, id)->datum;
    if ( object->object.type )
        return 0.0f;
    return object->object.current_shield_damage;
}
