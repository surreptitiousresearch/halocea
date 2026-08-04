/* object_align_marker_to_matrix @0x836EBF18 — reposition/reorient an object so that one of its markers
 * lands exactly on a target transform.
 *
 * It builds the object's current transform from its position/forward/up, then composes
 *   result = destination_matrix * inverse(inverse(object_transform) * child_marker_transform)
 * which is the object transform that places the child marker at destination_matrix. The object's position
 * and forward are read straight out of the composed matrix; the up vector is rebuilt as the cross product
 * of the forward (row 0) and binormal (row 2) rows to keep the basis orthogonal. Forward and up are then
 * renormalized (skipped if degenerate). */

#include <math.h>
#include "headers/object_datum.h"
#include "headers/object_marker.h"
#include "headers/real_matrix4x3.h"

#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
extern void matrix4x3_from_point_and_vectors(real_matrix4x3 *matrix, const real_point3d *point, const real_vector3d *forward, const real_vector3d *up);
extern void matrix4x3_inverse(const real_matrix4x3 *matrix, real_matrix4x3 *result);
extern void matrix4x3_multiply(const real_matrix4x3 *a, const real_matrix4x3 *b, real_matrix4x3 *result);

void object_align_marker_to_matrix(object_datum *object, const object_marker *child_marker,
                                   const real_matrix4x3 *destination_matrix)
{
    real_matrix4x3 result;
    real_matrix4x3 temp;

    matrix4x3_from_point_and_vectors(&result, &object->object.position,
                                     &object->object.forward, &object->object.up);
    matrix4x3_inverse(&result, &temp);
    matrix4x3_multiply(&temp, &child_marker->matrix, &temp);
    matrix4x3_inverse(&temp, &temp);
    matrix4x3_multiply(destination_matrix, &temp, &result);

    /* capture the forward (row 0) and binormal (row 2) rows before overwriting the object's basis */
    float f00 = result.n[0][0], f01 = result.n[0][1], f02 = result.n[0][2];
    float f20 = result.n[2][0], f21 = result.n[2][1], f22 = result.n[2][2];

    object->object.position.n[0] = result.n[3][0];
    object->object.position.n[1] = result.n[3][1];
    object->object.position.n[2] = result.n[3][2];

    object->object.forward.n[0] = f00;
    object->object.forward.n[1] = f01;
    object->object.forward.n[2] = f02;

    /* up = forward x binormal (component expansion, matching the binary's instruction order) */
    object->object.up.n[1] = (((f21 * f00) - (f01 * f20)) * f00) - (f02 * ((f22 * f01) - (f02 * f21)));
    object->object.up.n[2] = (f01 * ((f22 * f01) - (f02 * f21))) - (((f02 * f20) - (f22 * f00)) * f00);
    object->object.up.n[0] = (((f02 * f20) - (f22 * f00)) * f02) - (((f21 * f00) - (f01 * f20)) * f01);

    float forward_length = sqrtf((object->object.forward.n[2] * object->object.forward.n[2])
                               + (object->object.forward.n[0] * object->object.forward.n[0])
                               + (object->object.forward.n[1] * object->object.forward.n[1]));
    if ( fabsf(forward_length) >= 0.000099999997f )
    {
        float inverse_length = 1.0f / forward_length;
        object->object.forward.n[0] *= inverse_length;
        object->object.forward.n[1] *= inverse_length;
        object->object.forward.n[2] *= inverse_length;
    }

    float up_length = sqrtf((object->object.up.n[2] * object->object.up.n[2])
                          + (object->object.up.n[0] * object->object.up.n[0])
                          + (object->object.up.n[1] * object->object.up.n[1]));
    if ( fabsf(up_length) >= 0.000099999997f )
    {
        float inverse_length = 1.0f / up_length;
        object->object.up.n[0] *= inverse_length;
        object->object.up.n[1] *= inverse_length;
        object->object.up.n[2] *= inverse_length;
    }
}
