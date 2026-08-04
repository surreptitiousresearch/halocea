/* object_get_origin @ 0x836EF300 — an object's world-space origin. For an unattached object this is
 * its stored position; for an attached object it is that position transformed by the parent's
 * attachment node matrix. */

#include "headers/real_point3d.h"
#include "headers/real_matrix4x3.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/blam_data_globals.h"

extern real_point3d *matrix4x3_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);

real_point3d *object_get_origin(int object_index, real_point3d *origin)
{
    object_datum *object =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;

    if ( object->object.parent_object_index == -1 )
    {
        *origin = object->object.position;
    }
    else
    {
        unsigned char parent_node = (unsigned char)object->object.parent_node_index;
        object_datum *parent = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum,
                                                  object->object.parent_object_index)->datum;
        matrix4x3_transform_point(
            (const real_matrix4x3 *)((char *)parent + parent->object.node_matrices.offset) + parent_node,
            &object->object.position, origin);
    }
    return origin;
}
