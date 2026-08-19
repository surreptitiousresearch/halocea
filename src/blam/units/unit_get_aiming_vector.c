/* unit_get_aiming_vector @0x836C98E0 — return the unit's current aiming direction
 * (unit_datum.unit.aiming_vector). */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"


void unit_get_aiming_vector(int unit_index, real_vector3d *aiming_vector)
{
    unit_datum *unit =
        (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    *aiming_vector = unit->unit.aiming_vector;
}
