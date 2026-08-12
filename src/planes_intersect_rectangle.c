/* planes_intersect_rectangle @0x837C4BC8 — classify a 3D AABB against a set of cull planes (half-spaces).
 * Tests all eight box corners against every plane: if all eight fall behind one plane the box is fully outside
 * (returns 0); if no corner is behind any plane it is fully inside (returns 2); otherwise it straddles
 * (returns 1). With no planes the box is trivially inside. Rectangle layout is [x0,x1,y0,y1,z0,z1]. */

#include <stdint.h>
#include "headers/real_rectangle3d.h"
#include "headers/real_plane3d.h"
#include "headers/intersection.h"

int16_t planes_intersect_rectangle(const real_rectangle3d *bounds, int16_t cull_plane_count, const real_plane3d *cull_planes)
{
    float x0 = bounds->n[0], x1 = bounds->n[1];
    float y0 = bounds->n[2], y1 = bounds->n[3];
    float z0 = bounds->n[4], z1 = bounds->n[5];

    int accumulated_behind = 0;
    for (int plane_index = 0; plane_index < cull_plane_count; plane_index = (int16_t)(plane_index + 1))
    {
        const real_plane3d *plane = &cull_planes[plane_index];
        float nx = plane->n.n[0];
        float ny = plane->n.n[1];
        float nz = plane->n.n[2];
        float d = plane->d;

        int corner_mask = 0;
        if (x0 * nx + y0 * ny + z0 * nz - d < 0.0f) corner_mask |= 0x01;
        if (x1 * nx + y0 * ny + z0 * nz - d < 0.0f) corner_mask |= 0x02;
        if (x0 * nx + y1 * ny + z0 * nz - d < 0.0f) corner_mask |= 0x04;
        if (x1 * nx + y1 * ny + z0 * nz - d < 0.0f) corner_mask |= 0x08;
        if (x0 * nx + y0 * ny + z1 * nz - d < 0.0f) corner_mask |= 0x10;
        if (x1 * nx + y0 * ny + z1 * nz - d < 0.0f) corner_mask |= 0x20;
        if (x0 * nx + y1 * ny + z1 * nz - d < 0.0f) corner_mask |= 0x40;
        if (x1 * nx + y1 * ny + z1 * nz - d < 0.0f) corner_mask |= 0x80;

        if (corner_mask == 255)
            return _intersection_out;
        accumulated_behind |= corner_mask;
    }

    return (accumulated_behind == 0) + 1;
}
