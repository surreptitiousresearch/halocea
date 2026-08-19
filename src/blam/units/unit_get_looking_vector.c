/* unit_get_looking_vector @0x836C9920 — return the unit's current looking direction (cached on the object at
 * +0x260). */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"


void unit_get_looking_vector(int unit_index, real_vector3d *looking_vector)
{
    unit_datum *unit =
        (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    *looking_vector = unit->unit.looking_vector; /* was *(real_vector3d*)(datum + 608); _unit_datum.looking_vector @0x6C */
}
