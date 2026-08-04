/* unit_gunned_by_ai @0x836CA8C8 — is the unit's gun manned by an AI? If the unit has a distinct gunner
 * object, follow to it; then report whether the controlling unit is AI-driven, i.e. has a valid actor
 * (actor_index != -1). The arithmetic in the binary is the compiler's branchless form of that compare. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/blam_data_globals.h"

int unit_gunned_by_ai(int unit_index)
{
    unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;

    if (unit->unit.gunner_object_index != -1)
        unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum,
                                                unit->unit.gunner_object_index)->datum;

    return unit->unit.actor_index != -1;
}
