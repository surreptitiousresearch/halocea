/* object_get_world_matrix @0x836EF4B0 — build the world transform of an object from its stored position
 * and forward/up vectors. If the object is parented to another object's node, post-multiplies by that
 * parent node's world matrix so the result is in absolute world space. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/real_matrix4x3.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"

extern void matrix4x3_from_point_and_vectors(real_matrix4x3 *matrix, const real_point3d *point, const real_vector3d *forward, const real_vector3d *up);
extern void matrix4x3_multiply(const real_matrix4x3 *a, const real_matrix4x3 *b, real_matrix4x3 *result);

real_matrix4x3 *object_get_world_matrix(int object_index, real_matrix4x3 *matrix)
{
    object_datum *object =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;

    matrix4x3_from_point_and_vectors(matrix,
            &object->object.position,
            &object->object.forward,
            &object->object.up);

    if ( object->object.parent_object_index != -1 )
    {
        object_datum *parent = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum,
                                                  object->object.parent_object_index)->datum;
        const real_matrix4x3 *parent_node_matrix =
                (const real_matrix4x3 *)((char *)parent + parent->object.node_matrices.offset)
                + object->object.parent_node_index;
        matrix4x3_multiply(parent_node_matrix, matrix, matrix);
    }
    return matrix;
}
