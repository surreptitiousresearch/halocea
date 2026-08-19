/* collision_leaf_test_vector @ 0x837E13D8 — find the surface a ray crosses within one BSP leaf at a
 * given splitting plane. Scans the leaf's bsp2d references for the one on `plane_index`, projects the
 * ray's hit point (point + t*vector) onto the plane's dominant axis (dropping the largest normal
 * component), and locates the containing surface via the plane's 2D BSP. When `test_surface`
 * is set the candidate must also pass collision_surface_test_point (breakable/transparent filtering).
 * Returns the surface index, or -1 if none qualifies.
 *
 * Deviation: the decompiler emitted ~20 phantom trailing stack parameters (a10..a29) — the real
 * function takes 9; the final one (the surface-test gate) is `test_surface`. The projection-axis
 * lookup global_projection3d_mappings[3][2][2] is indexed here as [axis][sign] (the decompiler had
 * flattened it to [0][2*axis+sign]).
 *
 * Leaf (8 bytes): +2 (word) bsp2d reference count, +4 first reference index. bsp2d reference
 * (8 bytes): +0 plane index (sign bit = plane facing), +4 bsp2d root node. */

#include <stdint.h>
#include "headers/collision_bsp.h"
#include "headers/collision_leaf.h"
#include "headers/bsp2d_reference.h"
#include "headers/real_plane3d.h"
#include "headers/real_point2d.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"

extern double __fabs(double x);
extern int bsp2d_test_point(const bsp2d *bsp, const real_point2d *point, int child_index);
extern uint8_t collision_surface_test_point(const collision_bsp *bsp, int16_t breakable_surface_count, const uint8_t *breakable_surface_flags, int surface_index, int16_t projection_axis, uint8_t projection_sign, const real_point2d *point);

int collision_leaf_test_vector(const collision_bsp *bsp, int16_t breakable_surface_count,
                               const uint8_t *breakable_surface_flags, const real_point3d *point,
                               const real_vector3d *vector, int leaf_index, int plane_index, float t,
                               uint8_t test_surface)
{
    const collision_leaf *leaf = &((const collision_leaf *)bsp->leaves.address)[leaf_index];
    int first_reference = leaf->first_bsp2d_reference_index;
    int reference_count = leaf->bsp2d_reference_count;
    int index = first_reference;
    int surface_index;

    if ( first_reference >= reference_count + first_reference )
        return -1;

    for ( ;; )
    {
        const bsp2d_reference *reference = &((const bsp2d_reference *)bsp->bsp2d_references.address)[index];
        if ( (reference->plane_designator & 0x7FFFFFFF) == plane_index )
        {
            float *plane = (float *)&((real_plane3d *)bsp->bsp3d.planes.address)[plane_index];
            float ax = (float)__fabs(plane[0]);
            float ay = (float)__fabs(plane[1]);
            float az = (float)__fabs(plane[2]);
            int16_t projection_axis;
            unsigned char projection_sign;
            real_point2d projected;
            float hit_point[4];

            if ( az < ay || az < ax )       /* z is not the dominant axis */
                projection_axis = (ay >= ax);
            else
                projection_axis = 2;

            hit_point[0] = vector->n[0] * t + point->n[0];
            hit_point[1] = vector->n[1] * t + point->n[1];
            hit_point[2] = vector->n[2] * t + point->n[2];
            projection_sign = (reference->plane_designator < 0) != (plane[projection_axis] > 0.0f);

            projected.n[0] = hit_point[global_projection3d_mappings[projection_axis][projection_sign][0]];
            projected.n[1] = hit_point[global_projection3d_mappings[projection_axis][projection_sign][1]];

            surface_index = bsp2d_test_point(&bsp->bsp2d, &projected, reference->root_index);
            if ( !test_surface
              || collision_surface_test_point(bsp, breakable_surface_count, breakable_surface_flags,
                                              surface_index, projection_axis, projection_sign, &projected) )
            {
                return surface_index;
            }
        }
        if ( ++index >= reference_count + first_reference )
            return -1;
    }
}
