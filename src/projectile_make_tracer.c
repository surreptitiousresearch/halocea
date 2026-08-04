#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/projectile_datum.h"
#include "headers/projectile_datum_flags.h"
#include "headers/blam_data_globals.h"


void projectile_make_tracer(int projectile_index)
{
    projectile_datum *object = (projectile_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, projectile_index)->datum;
    object->projectile.flags |= (1u << _projectile_tracer_bit);
}
