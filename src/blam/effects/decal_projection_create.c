/* decal_projection_create @0x83740878 — build the projection state used to clip a decal onto BSP surfaces.
 * Given the decal's world basis (a 4x3 matrix: rows 0/1 are the u/v tangent axes, row 2 the projection
 * normal, row 3 the world origin) and its 2D extent rectangle, it: copies the basis, records the extent,
 * derives the projection plane (normal = basis row 2, distance = normal · origin), picks the dominant world
 * axis to flatten along (and the sign of the normal on that axis), projects the four extent corners into
 * world space and drops them onto the two non-dominant axes (selected via global_projection3d_mappings) to
 * get four 2D decal points, and finally computes the u/v texture axes and the reciprocal-area texture scale.
 *
 * The corner→2D mapping selects, for each (axis, sign) pair, which two of the three world coordinates form
 * the 2D point — global_projection3d_mappings[axis][sign][0..1] indexing a world[3].
 * DEVIATION: the decompiler flattened the table to [0][2*axis+sign][c]; the DB applied type
 * @0x821145B4 is const __int16[3][2][2], so the two leading subscripts are (axis, sign). */

#include <stdint.h>
#include <string.h>
#include "headers/real_matrix4x3.h"
#include "headers/real_rectangle2d.h"
#include "headers/decal_projection.h"
#include "headers/blam_data_globals.h"


extern double __fabs(double x);

void decal_projection_create(const real_matrix4x3 *basis, const real_rectangle2d *extent,
        decal_projection *projection)
{
    memcpy(projection, basis, sizeof(real_matrix4x3));   /* 0x34 — the basis matrix leads the struct */
    projection->extent = *extent;

    projection->plane.normal.n[0] = basis->n[2][0];
    projection->plane.normal.n[1] = basis->n[2][1];
    projection->plane.normal.n[2] = basis->n[2][2];
    projection->plane.distance =
            (projection->plane.normal.n[0] * basis->n[3][0])
          + ((basis->n[3][2] * projection->plane.normal.n[2])
                  + (basis->n[3][1] * projection->plane.normal.n[1]));

    /* dominant projection axis = the world axis with the largest |normal| component */
    float abs_x = __fabs(basis->n[2][0]);
    float abs_y = __fabs(basis->n[2][1]);
    float abs_z = __fabs(basis->n[2][2]);
    int16_t axis;
    if ( abs_z < abs_y || abs_z < abs_x )
        axis = abs_y >= abs_x;   /* y-dominant → 1, else x-dominant → 0 */
    else
        axis = 2;                /* z-dominant */
    projection->axis = axis;
    projection->sign = projection->plane.normal.n[axis] > 0.0f;

    int u_axis = global_projection3d_mappings[projection->axis][projection->sign][0];
    int v_axis = global_projection3d_mappings[projection->axis][projection->sign][1];
    float world[3];

    /* corner 0 = (x0, y0) */
    world[0] = (extent->n[0] * basis->n[0][0] + basis->n[1][0] * extent->n[2]) + basis->n[3][0];
    world[1] = ((extent->n[0] * basis->n[0][1]) + (extent->n[2] * basis->n[1][1]))
             + basis->n[3][1];
    world[2] = ((extent->n[0] * basis->n[0][2]) + (extent->n[2] * basis->n[1][2]))
             + basis->n[3][2];
    projection->decal_points2d[0].n[0] = world[u_axis];
    projection->decal_points2d[0].n[1] = world[v_axis];

    /* corner 1 = (x1, y0) */
    world[0] = (extent->n[1] * basis->n[0][0] + basis->n[1][0] * extent->n[2]) + basis->n[3][0];
    world[1] = ((extent->n[1] * basis->n[0][1]) + (extent->n[2] * basis->n[1][1]))
             + basis->n[3][1];
    world[2] = ((extent->n[1] * basis->n[0][2]) + (extent->n[2] * basis->n[1][2]))
             + basis->n[3][2];
    projection->decal_points2d[1].n[0] = world[u_axis];
    projection->decal_points2d[1].n[1] = world[v_axis];

    /* corner 2 = (x1, y1) */
    world[0] = (extent->n[1] * basis->n[0][0] + basis->n[1][0] * extent->n[3]) + basis->n[3][0];
    world[1] = ((extent->n[3] * basis->n[1][1]) + (extent->n[1] * basis->n[0][1]))
             + basis->n[3][1];
    world[2] = ((extent->n[3] * basis->n[1][2]) + (extent->n[1] * basis->n[0][2]))
             + basis->n[3][2];
    projection->decal_points2d[2].n[0] = world[u_axis];
    projection->decal_points2d[2].n[1] = world[v_axis];

    /* corner 3 = (x0, y1) */
    world[0] = (extent->n[0] * basis->n[0][0] + basis->n[1][0] * extent->n[3]) + basis->n[3][0];
    world[1] = ((extent->n[3] * basis->n[1][1]) + (extent->n[0] * basis->n[0][1]))
             + basis->n[3][1];
    world[2] = ((extent->n[3] * basis->n[1][2]) + (extent->n[0] * basis->n[0][2]))
             + basis->n[3][2];
    projection->decal_points2d[3].n[0] = world[u_axis];
    projection->decal_points2d[3].n[1] = world[v_axis];

    projection->texture_u_axis.n[0] = projection->decal_points2d[1].n[0] - projection->decal_points2d[0].n[0];
    projection->texture_u_axis.n[1] = projection->decal_points2d[1].n[1] - projection->decal_points2d[0].n[1];
    projection->texture_v_axis.n[0] = projection->decal_points2d[3].n[0] - projection->decal_points2d[0].n[0];
    projection->texture_v_axis.n[1] = projection->decal_points2d[3].n[1] - projection->decal_points2d[0].n[1];
    projection->texture_scale =
            1.0f / ((projection->texture_v_axis.n[1] * projection->texture_u_axis.n[0])
                         - (projection->texture_u_axis.n[1] * projection->texture_v_axis.n[0]));
}
