/* item_align_to_normal_and_point @0x83757FB0 — reorient a dropped item so its "ground point" marker's up axis
 * aligns with a surface normal, positioning it at a point. It resolves the item's "ground point" marker, forms
 * the shortest-arc quaternion that rotates the marker's up vector onto the surface normal (w = sqrt((1+dot)*2)/2,
 * axis = (normal x marker.up)/that), and rotates the marker's forward vector by it to get the new forward. When
 * the normal is nearly antiparallel to the marker up (the sqrt term collapses to ~0), it instead derives the
 * new forward directly from a double cross product against the marker forward. It then builds an alignment
 * matrix from the point, new forward, and normal, snaps the object's marker to it, and returns the resulting
 * object origin (object floats [23..25], i.e. +92..+100) through new_position. When position/new_position are
 * null they default to the marker's own object-space position.
 *
 * Object resolved via the salted object-header stride; matrix rows use the named real_matrix4x3 union fields
 * (forward = n[0], up = n[2]). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_marker.h"
#include "headers/real_quaternion.h"
#include "headers/real_matrix4x3.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/object_datum.h"
#include "headers/object_header_datum.h"
#include "headers/blam_data_globals.h"
extern float __fsqrts(float);

extern int16_t object_get_marker_by_name(int object_index, const char *name, object_marker *markers, int16_t maximum_marker_count);
extern void quaternion_transform_point(const real_quaternion *q, const real_point3d *p, real_point3d *result);
extern float normalize3d(real_vector3d *v);
extern void matrix4x3_from_point_and_vectors(real_matrix4x3 *matrix, const real_point3d *point, const real_vector3d *forward, const real_vector3d *up);
extern void object_align_marker_to_matrix(object_datum *object, const object_marker *child_marker, const real_matrix4x3 *destination_matrix);

void item_align_to_normal_and_point(int item_index, const real_vector3d *normal, const real_point3d *position, real_point3d *new_position)
{
    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, item_index)->datum;

    object_marker marker;
    real_point3d new_forward;
    real_matrix4x3 alignment_matrix[2];

    if ( !object_get_marker_by_name(item_index, "ground point", &marker, 1) )
        return;

    if ( !position )
        position = &marker.matrix.position;
    if ( !new_position )
        new_position = &marker.matrix.position;

    float nx = normal->n[0];
    float ny = normal->n[1];
    float half_scale = __fsqrts(((((ny * marker.matrix.up.n[1])
                + ((nx * marker.matrix.up.n[0]) + (normal->n[2] * marker.matrix.up.n[2])))
                + (float)1.0) * (float)2.0));

    if ( half_scale <= 0.01 )
    {
        /* normal ~ antiparallel to marker up: derive new forward from a double cross with marker forward */
        float cross_fwd = (nx * marker.matrix.forward.n[1]) - (ny * marker.matrix.forward.n[0]);
        float scaled = normal->n[2]
            * ((ny * marker.matrix.forward.n[2]) - (normal->n[2] * marker.matrix.forward.n[1]));
        new_forward.n[0] = (normal->n[2]
                * ((normal->n[2] * marker.matrix.forward.n[0]) - (nx * marker.matrix.forward.n[2])))
            - (ny * ((nx * marker.matrix.forward.n[1]) - (ny * marker.matrix.forward.n[0])));
        new_forward.n[2] = (ny
                * ((ny * marker.matrix.forward.n[2]) - (normal->n[2] * marker.matrix.forward.n[1])))
            - (nx * ((normal->n[2] * marker.matrix.forward.n[0]) - (nx * marker.matrix.forward.n[2])));
        new_forward.n[1] = (nx * cross_fwd) - scaled;
    }
    else
    {
        float nz = normal->n[2];
        float inverse_scale = (float)1.0 / half_scale;
        real_quaternion rotation;
        rotation.w = half_scale * (float)0.5;
        rotation.v.n[0] = ((nz * marker.matrix.up.n[1]) - (ny * marker.matrix.up.n[2])) * inverse_scale;
        rotation.v.n[1] = ((nx * marker.matrix.up.n[2]) - (nz * marker.matrix.up.n[0])) * inverse_scale;
        rotation.v.n[2] = ((ny * marker.matrix.up.n[0]) - (nx * marker.matrix.up.n[1])) * inverse_scale;
        quaternion_transform_point(&rotation, (const real_point3d *)&marker.matrix.forward, &new_forward);
    }

    normalize3d((real_vector3d *)&new_forward);
    matrix4x3_from_point_and_vectors(alignment_matrix, position, (const real_vector3d *)&new_forward, normal);
    object_align_marker_to_matrix(object, &marker, alignment_matrix);

    new_position->n[0] = object->object.position.n[0];
    new_position->n[1] = object->object.position.n[1];
    new_position->n[2] = object->object.position.n[2];
}
