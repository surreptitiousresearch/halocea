/* unit_scripting_get_grenade_count @0x83800A30 — return the total number of grenades a unit is carrying,
 * summed across both grenade types (unit.grenade_counts[2] @+0x31E), or 0 if the object is not a unit. */

#include "headers/unit_datum.h"
#include "headers/object_type.h"
#include "headers/unit_grenade_type.h"

extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);

int unit_scripting_get_grenade_count(int unit_index)
{
    unit_datum *unit = object_try_and_get_and_verify_type(unit_index, object_mask_unit);
    if ( !unit )
        return 0;
    __int16 total = 0;
    for ( int type = 0; type < NUMBER_OF_UNIT_GRENADE_TYPES; type = (__int16)(type + 1) )
        total = (__int16)(unit->unit.grenade_counts[type] + total);
    return total;
}
