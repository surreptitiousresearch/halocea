/* object_get_render_bounding_sphere @0x836E5BD0 — object render bounding sphere: center from object data (+0xA0),
 * but radius from the object's tag definition (float at +0x104, i.e. word index 65 of the tag instance data). */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/_object_definition.h"
#include "headers/real_point3d.h"
#include "headers/blam_data_globals.h"


void object_get_render_bounding_sphere(int object_index, real_point3d *center, float *radius)
{
    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    *center = object->object.bounding_sphere_center;  /* +160 = object_datum.object.bounding_sphere_center (0x04 + 0x9C) */
    /* radius is the object tag definition's render_bounding_radius (+0x104) */
    *radius = TAG_GET(_object_definition, object->definition_index)->render_bounding_radius;
}
