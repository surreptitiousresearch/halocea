/* render_frustum_triangle_visible @0x8376C8F8 — conservative test of whether a triangle is potentially visible
 * within a render frustum. Builds the outside-plane flag bits for each of the three vertices: if any vertex is
 * inside every plane (flags == 0) the triangle is visible; otherwise the triangle is visible unless all three
 * vertices lie outside a common plane (the AND of the low-6-bit plane masks is non-zero). */

#include <stdint.h>
#include "headers/render_frustum.h"
#include "headers/real_point3d.h"

extern int16_t render_frustum_build_point_flags(const render_frustum *frustum, const real_point3d *point);

uint8_t render_frustum_triangle_visible(const render_frustum *frustum, const real_point3d *p0, const real_point3d *p1, const real_point3d *p2)
{
    int16_t flags0 = render_frustum_build_point_flags(frustum, p0);
    if (!flags0)
        return 1;

    int16_t flags1 = render_frustum_build_point_flags(frustum, p1);
    if (!flags1)
        return 1;

    int16_t flags2 = render_frustum_build_point_flags(frustum, p2);
    if (!flags2)
        return 1;

    return (int16_t)((flags0 & 0x3F) & flags1 & flags2) == 0;
}
