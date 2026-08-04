/* unit_scripting_unit_gunner @0x836CA1D8 — return the unit gunning the given vehicle/unit (unit data +0x328),
 * or -1 if the object is not a unit. */

#include "headers/unit_datum.h"
#include "headers/object_type.h"

extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);

int unit_scripting_unit_gunner(int unit_index)
{
    unit_datum *unit = object_try_and_get_and_verify_type(unit_index, object_mask_unit);
    if ( unit )
        return unit->unit.gunner_object_index;
    return -1;
}
