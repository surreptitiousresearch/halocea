/* render_object_shadow_end @0x836E63E8 */
/* render_object_shadow_end 0x836E63E8 — close an object's shadow pass by submitting its shadow volume.
 *
 * Builds, on the stack, a single contiguous float[32] describing an oriented shadow box derived from the
 * object's shadow_matrix (an affine basis: three rows + an origin) scaled by shadow_bounding_radius, then
 * hands it to structure_render_shadow. The block is consumed as a real_rectangle3d (the world-space AABB of
 * the box, floats [0..5]) immediately followed — after a two-float gap [6..7] — by six real_plane3d clip
 * planes (floats [8..31], the +/- faces of the box along each of the three basis rows). The +/- projection
 * axis (basis row 2) is the light/extrusion direction, so its faces and AABB contribution use the asymmetric
 * near 0.5 / far 4.0 scaling, while the two lateral rows use the symmetric radius.
 *
 * DEVIATION (decompiler): the structure_render_shadow call was garbled — args 3/4/5 printed as LODWORD(v11) /
 * (__int16)v54 / (const real_plane3d *)6. The real call passes the AABB pointer (&shadow_volume[0]), the
 * literal polyhedra count 6, and the plane pointer (&shadow_volume[8]); the matching DB prototype is used.
 * The decompiler also split the one stack block into v54[17] + v55..v69; reunified here as shadow_volume[32]. */

#include <stdint.h>
#include "headers/object_render_data.h"
#include "headers/real_matrix4x3.h"
#include "headers/real_point3d.h"
#include "headers/real_rectangle3d.h"
#include "headers/real_plane3d.h"

extern void structure_render_shadow(const real_point3d *bounding_sphere_center, float bounding_radius, const real_rectangle3d *bounding_box, int16_t bounding_polyhedra_count, const real_plane3d *bounding_polyhedra);
extern void rasterizer_environment_shadow_end(void);

void render_object_shadow_end(object_render_data *data)
{
    const real_matrix4x3 *shadow_matrix = &data->shadow_matrix;
    const real_point3d *shadow_origin = (const real_point3d *)&shadow_matrix->n[3];
    float radius = data->shadow_bounding_radius;

    /* The three basis rows. Row 2 is the projection (extrusion) axis — its near/far span is asymmetric. */
    float row0_x = shadow_matrix->n[0][0], row0_y = shadow_matrix->n[0][1], row0_z = shadow_matrix->n[0][2];
    float row1_x = shadow_matrix->n[1][0], row1_y = shadow_matrix->n[1][1], row1_z = shadow_matrix->n[1][2];
    float axis_x = shadow_matrix->n[2][0], axis_y = shadow_matrix->n[2][1], axis_z = shadow_matrix->n[2][2];
    float origin_x = shadow_origin->x, origin_y = shadow_origin->y, origin_z = shadow_origin->z;

    /* Origin projected onto each basis axis: dot(row_i, origin). */
    float origin_dot_row0 = (origin_x * row0_x) + ((row0_y * origin_y) + (row0_z * origin_z));
    float origin_dot_row1 = (origin_x * row1_x) + ((row1_y * origin_y) + (row1_z * origin_z));
    float origin_dot_axis = (origin_x * axis_x) + ((axis_y * origin_y) + (axis_z * origin_z));

    float shadow_volume[32];
    real_rectangle3d *world_bounds = (real_rectangle3d *)&shadow_volume[0];  /* AABB,  floats [0..5]  */
    real_plane3d *box_planes = (real_plane3d *)&shadow_volume[8];            /* planes, floats [8..31] */

    /* Six clip planes: +/- of each basis row. The projection axis (row 2) is near 0.5 / far 4.0; the two
     * lateral rows (0,1) are symmetric at radius. */
    box_planes[0].normal.i = axis_x;  box_planes[0].normal.j = axis_y;  box_planes[0].normal.k = axis_z;
    box_planes[0].distance = -((radius * 0.5f) - origin_dot_axis);
    box_planes[1].normal.i = -axis_x; box_planes[1].normal.j = -axis_y; box_planes[1].normal.k = -axis_z;
    box_planes[1].distance = -((radius * 4.0f) - (-origin_dot_axis));
    box_planes[2].normal.i = row0_x;  box_planes[2].normal.j = row0_y;  box_planes[2].normal.k = row0_z;
    box_planes[2].distance = origin_dot_row0 - radius;
    box_planes[3].normal.i = -row0_x; box_planes[3].normal.j = -row0_y; box_planes[3].normal.k = -row0_z;
    box_planes[3].distance = -origin_dot_row0 - radius;
    box_planes[4].normal.i = row1_x;  box_planes[4].normal.j = row1_y;  box_planes[4].normal.k = row1_z;
    box_planes[4].distance = origin_dot_row1 - radius;
    box_planes[5].normal.i = -row1_x; box_planes[5].normal.j = -row1_y; box_planes[5].normal.k = -row1_z;
    box_planes[5].distance = -origin_dot_row1 - radius;

    /* World-space AABB. Each lateral half-extent is |row0_c| + |row1_c|; the projection axis adds an
     * asymmetric near(-0.5)/far(4.0) span whose world direction depends on the sign of that component. */
    float lateral_x = ((row0_x < 0.0f) ? -row0_x : row0_x) + ((row1_x < 0.0f) ? -row1_x : row1_x);
    float lateral_y = ((row0_y < 0.0f) ? -row0_y : row0_y) + ((row1_y < 0.0f) ? -row1_y : row1_y);
    float lateral_z = ((row0_z < 0.0f) ? -row0_z : row0_z) + ((row1_z < 0.0f) ? -row1_z : row1_z);

    world_bounds->x0 = origin_x + radius * (((axis_x > 0.0f) ? (axis_x * -0.5f) : (axis_x * 4.0f)) - lateral_x);
    world_bounds->x1 = origin_x + radius * (((axis_x > 0.0f) ? (axis_x * 4.0f) : (axis_x * -0.5f)) + lateral_x);
    world_bounds->y0 = origin_y + radius * (((axis_y > 0.0f) ? (axis_y * -0.5f) : (axis_y * 4.0f)) - lateral_y);
    world_bounds->y1 = origin_y + radius * (((axis_y > 0.0f) ? (axis_y * 4.0f) : (axis_y * -0.5f)) + lateral_y);
    /* DEVIATION (faithful quirk): the Z axis is NOT sign-branched like X/Y — the original assumes the
     * projection axis points down (axis_z <= 0), so min uses *4.0 and max uses *-0.5 unconditionally. */
    world_bounds->z0 = origin_z + radius * ((axis_z * 4.0f) - lateral_z);
    world_bounds->z1 = origin_z + radius * (lateral_z - (axis_z * 0.5f));

    structure_render_shadow(shadow_origin, radius * 4.0f, world_bounds, 6, box_planes);
    rasterizer_environment_shadow_end();
}
