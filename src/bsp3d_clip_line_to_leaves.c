/* bsp3d_clip_line_to_leaves @0x837EA268 — recursively clip a line segment [p0,p1] down a bsp3d tree, invoking
 * `handler` once per leaf the segment (or a clipped sub-segment of it) passes through, and returning the number
 * of leaves reached. At each node the signed distances of both endpoints to the split plane are computed; if the
 * segment straddles the plane the crossing point is found parametrically. Each side is descended when it holds
 * part of the segment (an endpoint on that side, or both endpoints coplanar), with the sub-segment endpoints
 * clipped to the crossing point on whichever end lies on the far side. Non-negative children are recursed;
 * leaf children (top bit set) fire `handler` with the leaf index; -1 children (no geometry) are skipped. This is
 * the line analogue of bsp3d_clip_polygon_to_leaves_recursive.
 *
 * DEVIATION: `node_count` is a global node-visit counter reset to 1 at the root (node_index 0) and incremented on
 * every deeper node — reproduced from the decompiler's `v14=0; if(node_index) v14=node_count; node_count=v14+1`.
 * epsilon = 0.00024414062 = 1/4096. Per-step (float) casts preserved for single-precision rounding. */

#include <stdint.h>
#include "headers/bsp3d.h"
#include "headers/bsp3d_node.h"
#include "headers/real_point3d.h"
#include "headers/real_plane3d.h"
#include "headers/blam_data_globals.h"


int bsp3d_clip_line_to_leaves(const bsp3d *bsp, int node_index, const real_point3d *p0, const real_point3d *p1,
        void (*handler)(const real_point3d *, const real_point3d *, int, void *), void *user_data)
{
    const bsp3d_node *node = &((const bsp3d_node *)bsp->nodes.address)[node_index];
    const real_plane3d *plane = &((const real_plane3d *)bsp->planes.address)[node->plane_index];

    float distance_p1 = (((plane->n.n[0] * p1->n[0])
                + ((plane->n.n[1] * p1->n[1]) + (plane->n.n[2] * p1->n[2])))
            - plane->d);
    float distance_p0 = (((p0->n[0] * plane->n.n[0])
                + ((plane->n.n[2] * p0->n[2]) + (plane->n.n[1] * p0->n[1])))
            - plane->d);

    node_count = node_index ? node_count + 1 : 1;

    uint8_t p0_side[2];   /* [0] = behind plane, [1] = in front */
    uint8_t p1_side[2];
    p0_side[0] = distance_p0 < -0.00024414062f;
    p0_side[1] = distance_p0 > 0.00024414062f;
    p1_side[0] = distance_p1 < -0.00024414062f;
    p1_side[1] = distance_p1 > 0.00024414062f;

    real_point3d split_point;
    if ( (p0_side[0] && p1_side[1]) || (p0_side[1] && p1_side[0]) )
    {
        float t = -((((plane->n.n[0] * p0->n[0])
                        + ((plane->n.n[2] * p0->n[2]) + (plane->n.n[1] * p0->n[1])))
                    - plane->d)
                / ((plane->n.n[0] * (p1->n[0] - p0->n[0]))
                        + ((plane->n.n[1] * (p1->n[1] - p0->n[1]))
                                + (plane->n.n[2] * (p1->n[2] - p0->n[2])))));
        split_point.n[0] = ((p1->n[0] - p0->n[0]) * t) + p0->n[0];
        split_point.n[1] = ((p1->n[1] - p0->n[1]) * t) + p0->n[1];
        split_point.n[2] = ((p1->n[2] - p0->n[2]) * t) + p0->n[2];
    }

    int leaf_count = 0;
    for ( int side = 0; side < 2; ++side )
    {
        int opposite = (side == 0);   /* 1 when side==0, 0 when side==1 */
        if ( !(p0_side[side] || p1_side[side] || (!p0_side[opposite] && !p1_side[opposite])) )
            continue;

        const real_point3d *segment_start = p0_side[opposite] ? &split_point : p0;
        const real_point3d *segment_end   = p1_side[opposite] ? &split_point : p1;
        int child = node->child_indices[side];  /* [0]=back, [1]=front */
        if ( child >= 0 )
        {
            leaf_count += bsp3d_clip_line_to_leaves(bsp, child, segment_start, segment_end, handler, user_data);
        }
        else if ( child != -1 )
        {
            if ( handler )
                handler(segment_start, segment_end, child & 0x7FFFFFFF, user_data);
            ++leaf_count;
        }
    }

    return leaf_count;
}
