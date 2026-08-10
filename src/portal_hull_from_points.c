/* portal_hull_from_points @0x837C4EE8 — project a world-space portal/mirror polygon into the camera's screen
 * space, producing a 2D convex hull for visibility testing. The camera's signed distance to the portal plane
 * (times the winding) decides the result: within 0.1 of the plane returns 2 (camera straddles it), behind it
 * returns 1 (back-facing, rejected). Otherwise the polygon vertices are transformed into view space, clipped
 * against the near screen plane, and perspective-projected (divide by -z) into the hull, emitted in winding
 * order (reversed when the camera is mirrored). Returns 0 when a usable hull (>= 3 vertices) was built, else 1.
 *
 * DEVIATION 1: the decompiler packed the 6th (winding, __int16) and 7th (hull pointer) arguments into a single
 * __int64; the disassembly shows r8 = winding direction (used both as the facing-sign multiplier and the vertex
 * walk step) and r9 = the output hull. Restored as two parameters.
 * DEVIATION 2: the decompiler rendered the clip's keep_coplanar argument as the string "neoffire_crouching";
 * the disassembly loads r10 = 1 (a stray symbol annotation), so keep_coplanar is 1. */

#include <stdint.h>
#include "headers/render_camera.h"
#include "headers/render_frustum.h"
#include "headers/real_plane3d.h"
#include "headers/real_point3d.h"
#include "headers/real_matrix4x3.h"
#include "headers/portal_hull.h"
#include "headers/portal_hull_from_portal_result.h"
#include "headers/ppc_intrinsics.h"
extern real_plane3d screen_plane;

extern real_point3d *matrix4x3_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);
extern int16_t convex_polygon3d_clip_to_plane(int16_t count, const real_point3d *points, const real_plane3d *plane, int16_t maximum_count, real_point3d *result, uint8_t *clipped, float epsilon, uint8_t keep_coplanar);

int16_t portal_hull_from_points(const render_camera *camera, const render_frustum *frustum, const real_plane3d *plane, int16_t vertex_count, const real_point3d *vertices, int16_t winding, portal_hull *hull)
{
    hull->vertex_count = 0;

    float camera_distance = (camera->position.n[0] * plane->n.n[0]
                             + (camera->position.n[2] * plane->n.n[2]
                                + camera->position.n[1] * plane->n.n[1]))
                            - plane->d;
    float facing = camera_distance * (float)winding;

    int16_t walk_step = winding;
    if (camera->mirrored)
        walk_step = -winding;

    if (__fabs(facing) < 0.1f)
        return _portal_hull_from_portal_degenerate;
    if (facing <= 0.0f)
        return _portal_hull_from_portal_discarded;

    real_point3d view_space[261];
    for (int i = 0; i < vertex_count; i = (int16_t)(i + 1))
        matrix4x3_transform_point(&frustum->world_to_view, &vertices[i], &view_space[i]);

    int16_t clipped_count = convex_polygon3d_clip_to_plane(vertex_count, view_space, &screen_plane, 256,
                                                           view_space, nullptr, 0.000099999997f, 1);
    hull->vertex_count = clipped_count;

    int source_index;
    int terminator;
    if (walk_step == 1)
    {
        source_index = 0;
        terminator = clipped_count;
    }
    else
    {
        source_index = clipped_count - 1;
        terminator = -1;
    }

    int output_index = 0;
    while (source_index != terminator)
    {
        float inverse_z = -1.0f / view_space[source_index].n[2];
        hull->vertices[output_index].n[0] = view_space[source_index].n[0] * inverse_z;
        hull->vertices[output_index].n[1] = view_space[source_index].n[1] * inverse_z;
        ++output_index;
        source_index = (int16_t)(source_index + walk_step);
    }

    return ((hull->vertex_count >= 0) + ((unsigned int)hull->vertex_count >= 3)) & 1;
}
