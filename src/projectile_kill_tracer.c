/* projectile_kill_tracer @0x83759098 */
/* projectile_kill_tracer — clear the projectile's tracer flag (projectile.flags _projectile_tracer_bit). */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/projectile_datum.h"
#include "headers/projectile_datum_flags.h"
#include "headers/blam_data_globals.h"


void projectile_kill_tracer(int projectile_index)
{
    projectile_datum *projectile = (projectile_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, projectile_index)->datum;
    projectile->projectile.flags &= ~(1u << _projectile_tracer_bit);
}
