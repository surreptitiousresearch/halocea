/* effect_marker_list_get_markers_by_name @0x836E2950 — collect the markers of the active effect
 * marker list whose name matches `name`, writing up to maximum_count object_markers into results
 * and returning the count found. If the list has a name table and a non-empty query, each named
 * marker is compared and matches are resolved via effect_marker_list_get_marker.
 *
 * If nothing matched (or the list has no names), a single synthetic marker is produced from the
 * list's stored point/forward: when the list is bound to an object node the point and forward are
 * pulled into node-local space, otherwise they are used directly. A perpendicular vector is derived
 * and normalized, and the marker's node_matrix is built from (point, forward, perpendicular).
 * Returns 1 in that fallback case. */

#include <stdint.h>
#include "headers/effect_marker_list.h"
#include "headers/object_marker.h"
#include "headers/real_matrix4x3.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"
#include "headers/ppc_intrinsics.h"


extern int  strcmp(const char *a, const char *b);
extern void effect_marker_list_get_marker(object_marker *result, const effect_marker_list *marker_list, int16_t effect_marker_index);
extern real_point3d *matrix4x3_inverse_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);
extern real_vector3d *matrix4x3_inverse_transform_normal(const real_matrix4x3 *matrix, const real_vector3d *normal, real_vector3d *result);
extern real_vector3d *perpendicular3d(const real_vector3d *a, real_vector3d *result);
extern void matrix4x3_from_point_and_vectors(real_matrix4x3 *matrix, const real_point3d *point, const real_vector3d *forward, const real_vector3d *up);

/* return respelled int->int16_t 2026-07-30: harmonizes the effect_build_locations get_markers_by_name
 * slot (int16_t) and sibling impl first_person_weapon_get_marker_by_name (int16_t); body returns a
 * marker count bounded by int16_t maximum_count (mr r3,r30, width-agnostic) (C4133) */
int16_t effect_marker_list_get_markers_by_name(int object_index, const char *name, object_marker *results, int16_t maximum_count)
{
    int16_t found = 0;
    const effect_marker_list *list = global_effect_marker_list;

    if (list->names && *name) /* has a name table and a non-empty query */
    {
        for (int16_t i = 0; found < maximum_count; ++i)
        {
            if (i >= list->count)
                break;
            if (strcmp(name, list->names[i]) == 0)
            {
                effect_marker_list_get_marker(&results[found], list, i);
                found = (int16_t)(found + 1);
                list = global_effect_marker_list;
            }
        }
    }

    if (found)
        return found;

    /* fallback: synthesize one marker from the list's stored point/forward */
    results->node_index = list->node_index;
    real_point3d  point;
    real_vector3d forward;
    if (list->node_matrix)
    {
        matrix4x3_inverse_transform_point(list->node_matrix, list->points, &point);
        matrix4x3_inverse_transform_normal(list->node_matrix, list->forwards, &forward);
    }
    else
    {
        point = list->points[0];
        forward = list->forwards[0];
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

    matrix4x3_from_point_and_vectors(&results->node_matrix, &point, &forward, &perpendicular);
    return 1;
}
