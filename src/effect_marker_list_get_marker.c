/* effect_marker_list_get_marker @0x836E17E8 — resolve a single marker (by index) of an effect marker
 * list into an object_marker. The marker's point and forward are pulled into node-local space when the
 * list is bound to an object node, otherwise used directly. A perpendicular vector is derived and
 * normalized, and the marker's node_matrix is built from (point, forward, perpendicular). */

#include <stdint.h>
#include "headers/effect_marker_list.h"
#include "headers/object_marker.h"
#include "headers/real_matrix4x3.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
extern float __fsqrts(float);

extern real_point3d *matrix4x3_inverse_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);
extern real_vector3d *matrix4x3_inverse_transform_normal(const real_matrix4x3 *matrix, const real_vector3d *normal, real_vector3d *result);
extern real_vector3d *perpendicular3d(const real_vector3d *a, real_vector3d *result);
extern void matrix4x3_from_point_and_vectors(real_matrix4x3 *matrix, const real_point3d *point, const real_vector3d *forward, const real_vector3d *up);

void effect_marker_list_get_marker(object_marker *result, const effect_marker_list *marker_list, int16_t effect_marker_index)
{
    result->node_index = marker_list->node_index;

    real_point3d  point;
    real_vector3d forward;
    if (marker_list->node_matrix)
    {
        matrix4x3_inverse_transform_point(marker_list->node_matrix,
                                          &marker_list->points[effect_marker_index], &point);
        matrix4x3_inverse_transform_normal(marker_list->node_matrix,
                                           &marker_list->forwards[effect_marker_index], &forward);
    }
    else
    {
        point = marker_list->points[effect_marker_index];
        forward = marker_list->forwards[effect_marker_index];
    }

    real_vector3d perpendicular;
    perpendicular3d(&forward, &perpendicular);
    float length = __fsqrts(perpendicular.n[2] * perpendicular.n[2]
                            + (perpendicular.n[0] * perpendicular.n[0]
                               + perpendicular.n[1] * perpendicular.n[1]));
    if (__fabs(length) >= 0.000099999997f)
    {
        float inverse_length = 1.0f / length;
        perpendicular.n[0] *= inverse_length;
        perpendicular.n[1] *= inverse_length;
        perpendicular.n[2] *= inverse_length;
    }

    matrix4x3_from_point_and_vectors(&result->node_matrix, &point, &forward, &perpendicular);
}
