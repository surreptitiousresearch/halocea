/* projectile_set_target_object_index @0x83759130 — writes a projectile object's
 * projectile.target_object_index. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/projectile_datum.h"
#include "headers/blam_data_globals.h"


void projectile_set_target_object_index(int projectile_index, int target_object_index)
{
    projectile_datum *projectile = (projectile_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, projectile_index)->datum;
    projectile->projectile.target_object_index = target_object_index;
}
