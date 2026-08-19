/* render_frustum_cube_visible @0x8376C980 */
/* render_frustum_cube_visible 0x8376C980 — test an axis-aligned box against a render frustum. Returns 0 if the
 * box is wholly outside, 2 if wholly inside, 1 if it straddles. Fast-rejects on the frustum/box world-bounds
 * AABB overlap, then classifies the 8 box corners against the 4 side planes (a corner's bit is set when it is
 * on the outside of that plane): if all corners share an outside plane the box is culled (0); if no corner is
 * outside any plane it is fully inside (2). For a straddle, when test_frustum_against_cube is set it also tests
 * the 5 frustum world vertices against the box AABB (separating-axis style) and culls if they all fall outside
 * one box face.
 *
 * DEVIATION: the decompiler indexed the frustum's 5 world vertices as frustum_bounds.n[3*i + 56]; that offset
 * lands on world_vertices[i], used directly here. The 8 corners are assembled into a scratch array at the same
 * (non-sequential) indices the original used. */

#include <stdint.h>
#include "headers/render_frustum.h"
#include "headers/real_rectangle3d.h"

int16_t render_frustum_cube_visible(const render_frustum *frustum, const real_rectangle3d *bounds, uint8_t test_frustum_against_cube)
{
    float bx0 = bounds->n[0], bx1 = bounds->n[1];
    float by0 = bounds->n[2], by1 = bounds->n[3];
    float bz0 = bounds->n[4], bz1 = bounds->n[5];
    float plane0_x, plane0_y, plane0_z, plane0_d;
    float plane1_x, plane1_y, plane1_z, plane1_d;
    float plane2_x, plane2_y, plane2_z, plane2_d;
    float plane3_x, plane3_y, plane3_z, plane3_d;
    int16_t corners_outside_common;
    int16_t corners_outside_any;
    int corner_index;
    float cube_corners[24];

    /* AABB fast reject against the frustum's world bounds */
    if ( frustum->world_bounds.n[1] < bx0 )
        return 0;
    if ( frustum->world_bounds.n[3] < by0 )
        return 0;
    if ( frustum->world_bounds.n[5] < bz0 )
        return 0;
    if ( frustum->world_bounds.n[0] > bx1 )
        return 0;
    if ( frustum->world_bounds.n[2] > by1 )
        return 0;
    if ( frustum->world_bounds.n[4] > bz1 )
        return 0;

    plane0_z = frustum->world_planes[0].normal.n[2];
    plane0_y = frustum->world_planes[0].normal.n[1];
    plane0_x = frustum->world_planes[0].normal.n[0];
    plane0_d = frustum->world_planes[0].distance;
    plane1_z = frustum->world_planes[1].normal.n[2];
    plane1_y = frustum->world_planes[1].normal.n[1];
    plane1_x = frustum->world_planes[1].normal.n[0];
    plane1_d = frustum->world_planes[1].distance;
    plane2_z = frustum->world_planes[2].normal.n[2];
    plane2_y = frustum->world_planes[2].normal.n[1];
    plane2_x = frustum->world_planes[2].normal.n[0];
    plane2_d = frustum->world_planes[2].distance;
    plane3_z = frustum->world_planes[3].normal.n[2];
    plane3_y = frustum->world_planes[3].normal.n[1];
    plane3_x = frustum->world_planes[3].normal.n[0];
    plane3_d = frustum->world_planes[3].distance;

    cube_corners[18] = bx0;
    cube_corners[21] = bx1;
    cube_corners[16] = by0;
    cube_corners[22] = by1;
    cube_corners[11] = bz0;
    cube_corners[23] = bz1;
    cube_corners[12] = bx0;
    cube_corners[6] = bx0;
    cube_corners[15] = bx1;
    cube_corners[9] = bx1;
    cube_corners[3] = bx1;
    cube_corners[13] = by0;
    cube_corners[4] = by0;
    cube_corners[19] = by1;
    cube_corners[10] = by1;
    cube_corners[7] = by1;
    cube_corners[8] = bz0;
    cube_corners[5] = bz0;
    cube_corners[2] = bz0;
    cube_corners[20] = bz1;
    cube_corners[17] = bz1;
    cube_corners[14] = bz1;

    corners_outside_common = 63;
    corners_outside_any = 0;
    corner_index = 0;
    do
    {
        float *corner = &cube_corners[3 * corner_index];
        float cy = corner[1];
        float cx = *corner;
        float cz = corner[2];
        int16_t flags;
        int16_t bit;

        flags = (((cz * plane0_z) + ((plane0_x * cx) + (cy * plane0_y)))
                      - plane0_d) > 0.0;
        bit = 2;
        if ( (((plane1_z * cz) + ((plane1_x * cx) + (plane1_y * cy)))
                   - plane1_d) <= 0.0 )
            bit = 0;
        flags |= bit;
        bit = 8;
        if ( (((plane2_z * cz) + ((plane2_x * cx) + (plane2_y * cy)))
                   - plane2_d) <= 0.0 )
            bit = 0;
        flags |= bit;
        bit = 4;
        if ( (((plane3_z * cz) + ((plane3_x * cx) + (plane3_y * cy)))
                   - plane3_d) <= 0.0 )
            bit = 0;
        flags |= bit;

        corner_index = (int16_t)(corner_index + 1);
        corners_outside_common &= flags;
        corners_outside_any |= flags;
    }
    while ( corner_index < 8 );

    if ( !corners_outside_any )
        return 2;
    if ( corners_outside_common )
        return 0;
    if ( !test_frustum_against_cube )
        return 1;

    /* reverse test: the 5 frustum world vertices vs the box AABB */
    int16_t vertices_outside_common = 63;
    int vertex_index = 0;
    do
    {
        const real_point3d *vertex = &frustum->world_vertices[vertex_index];
        float vx = vertex->n[0];
        float vy = vertex->n[1];
        float vz = vertex->n[2];
        int16_t flags;
        int16_t bit;

        flags = vx <= bx0;
        bit = 2;
        if ( vx < bx1 )
            bit = 0;
        flags |= bit;
        bit = 8;
        if ( vy > by0 )
            bit = 0;
        flags |= bit;
        bit = 4;
        if ( vy < by1 )
            bit = 0;
        flags |= bit;
        bit = 16;
        if ( vz > bz0 )
            bit = 0;
        flags |= bit;
        bit = 32;
        if ( vz < bz1 )
            bit = 0;

        vertex_index = (int16_t)(vertex_index + 1);
        vertices_outside_common &= flags | bit;
    }
    while ( vertex_index < 5 );

    if ( !vertices_outside_common )
        return 1;
    return 0;
}
