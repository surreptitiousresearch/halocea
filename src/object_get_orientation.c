/* object_get_orientation @ 0x836EF3B8 — an object's world-space forward and up vectors. Unattached
 * objects return their stored vectors directly; attached objects return them rotated by the parent's
 * attachment node matrix. Either output pointer may be null. */

#include "headers/real_vector3d.h"
#include "headers/real_matrix4x3.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/blam_data_globals.h"

extern real_vector3d *matrix4x3_transform_normal(const real_matrix4x3 *matrix, const real_vector3d *normal, real_vector3d *result);

void object_get_orientation(int object_index, real_vector3d *forward, real_vector3d *up)
{
    object_datum *object =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;

    if ( object->object.parent_object_index == -1 )
    {
        if ( forward )
            *forward = object->object.forward;
        if ( up )
            *up = object->object.up;
    }
    else
    {
        object_datum *parent = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum,
                                                  object->object.parent_object_index)->datum;
        const real_matrix4x3 *node_matrix =
            (const real_matrix4x3 *)((char *)parent + parent->object.node_matrices.offset)
            + object->object.parent_node_index;
        if ( forward )
            matrix4x3_transform_normal(node_matrix, &object->object.forward, forward);
        if ( up )
            matrix4x3_transform_normal(node_matrix, &object->object.up, up);
    }
}
