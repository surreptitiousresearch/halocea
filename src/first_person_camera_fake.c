/* first_person_camera_fake @0x837C25E0 — build a first-person observer command for a unit using the unit's
 * stored aiming vector (object+572) as the look direction. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/observer_command.h"
#include "headers/unit_datum.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"

#include "headers/real_vector3d.h"
extern void first_person_camera_for_unit_and_vector(int unit_index, const real_vector3d *forward, observer_command *result);

void first_person_camera_fake(int unit_index, observer_command *result)
{
    unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
    /* unit+572 = _unit_datum.aiming_vector (unit block @0x1F4, aiming_vector @0x48) */
    first_person_camera_for_unit_and_vector(unit_index, &unit->unit.aiming_vector, result);
}
