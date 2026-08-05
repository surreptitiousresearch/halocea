/* grow_clipping_rectangle_by_portal_hull @0x837C51DC — expands a 2D clipping rectangle (min-x, max-x,
 * min-y, max-y) to enclose every vertex of a portal hull. */

#include <stdint.h>
#include "headers/real_rectangle2d.h"
#include "headers/portal_hull.h"

void grow_clipping_rectangle_by_portal_hull(real_rectangle2d *rectangle, const portal_hull *hull)
{
    const real_point2d *vertices = hull->vertices;

    for ( int16_t i = 0; i < hull->vertex_count; ++i )
    {
        if ( rectangle->n[0] > vertices[i].n[0] )
            rectangle->n[0] = vertices[i].n[0];
        if ( rectangle->n[1] < vertices[i].n[0] )
            rectangle->n[1] = vertices[i].n[0];
        if ( rectangle->n[2] > vertices[i].n[1] )
            rectangle->n[2] = vertices[i].n[1];
        if ( rectangle->n[3] < vertices[i].n[1] )
            rectangle->n[3] = vertices[i].n[1];
    }
}
