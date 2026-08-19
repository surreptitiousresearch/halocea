/* object_get_bounding_sphere @0x836E5B88 — read an object's current bounding sphere: center (object data +0xA0)
 * and radius (object data +0xAC). */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/real_point3d.h"
#include "headers/blam_data_globals.h"


void object_get_bounding_sphere(int object_index, real_point3d *center, float *radius)
{
    object_datum *object_data = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    *center = object_data->object.bounding_sphere_center;
    *radius = object_data->object.bounding_sphere_radius;
}
