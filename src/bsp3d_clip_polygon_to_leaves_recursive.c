/* bsp3d_clip_polygon_to_leaves_recursive @0x837EA530 — recursively split a convex polygon down a bsp3d
 * tree, invoking `handler` once per leaf the polygon (or a clipped fragment of it) lands in. At each node:
 * if every vertex lies within `epsilon` of the split plane, the whole polygon is coplanar with it — no
 * clip is needed, just a winding-order test to pick which side to recurse down, folding the result into
 * `on_node_designator`'s sign bit. Otherwise the polygon is clipped against the plane and its negation to
 * produce a back and a front fragment, each recursed into its respective child (or, at a leaf child,
 * handed to `handler`).
 *
 * DEVIATION 1: the DB's own prototype (arg_count 8) has no 9th parameter and `epsilon` is `float`, not the
 * `double`/phantom-`a9` the decompiler showed — the decompiler over-read the stack past this function's
 * real argument list; disasm confirms exactly 8 register-passed args (bsp..user_data) with no spill.
 * DEVIATION 2: the decompiler swapped `handler` and `user_data` in its display (calling the wrong one
 * "user_data" and inventing a phantom trailing "a9" for the real user_data) — disasm shows r26 (mtctr'd
 * for the indirect call) is this function's own `handler` parameter, and r25 is `user_data`, passed as the
 * callback's real 5th argument. The recursive call is a plain 8-argument passthrough, not the 5-argument
 * cast-from-`v29*4` garbage the decompiler printed.
 * FAITHFUL QUIRK: `keep_coplanar` (both convex_polygon3d_clip_to_plane calls) is never actually loaded —
 * r9 still holds `bsp->planes.address` from the node/plane lookup earlier in the function and is passed
 * through unchanged, so both clip calls receive the low byte of that pointer as `keep_coplanar` rather than
 * any real flag. Reproduced verbatim (not a defensible flag, but what the compiled binary does). */

#include <stdint.h>
#include <stddef.h>
#include "headers/bsp3d.h"
#include "headers/bsp3d_node.h"
#include "headers/real_point3d.h"
#include "headers/real_plane3d.h"
#include "headers/ppc_intrinsics.h"

extern int16_t convex_polygon3d_clip_to_plane(int16_t count, const real_point3d *points, const real_plane3d *plane, int16_t maximum_count, real_point3d *result, uint8_t *clipped, float epsilon, uint8_t keep_coplanar);

int bsp3d_clip_polygon_to_leaves_recursive(
        const bsp3d        *bsp,
        int                  node_index,
        int                  on_node_designator,
        const real_point3d  *vertices,
        int16_t              vertex_count,
        float                epsilon,
        void (*handler)(const real_point3d *, int16_t, int, int, void *),
        void                *user_data)
{
    const bsp3d_node *node  = &((const bsp3d_node *)bsp->nodes.address)[node_index];
    const char       *planes_address = (const char *)bsp->planes.address;
    const real_plane3d *plane = (const real_plane3d *)&planes_address[16 * node->plane_index];
    /* FAITHFUL QUIRK source: this same pointer, truncated, is reused as keep_coplanar below. */
    uint8_t keep_coplanar = (uint8_t)(unsigned int)planes_address;

    int16_t i;
    for (i = 0; i < vertex_count; ++i)
    {
        float dist = plane->n.n[0] * vertices[i].n[0]
                   + plane->n.n[1] * vertices[i].n[1]
                   + plane->n.n[2] * vertices[i].n[2]
                   - plane->d;
        if (__fabs(dist) >= epsilon)
            break;
    }

    int16_t side_count[2];
    const real_point3d *side_vertices[2];
    real_point3d back_clipped[64];
    real_point3d front_clipped[64];

    if (i == vertex_count)
    {
        /* whole polygon is coplanar with the split plane; pick a side by winding order */
        int front_facing = (plane->n.n[0] * ((vertices[2].n[1] - vertices[0].n[1]) * (vertices[1].n[2] - vertices[0].n[2])
                                                 - (vertices[2].n[2] - vertices[0].n[2]) * (vertices[1].n[1] - vertices[0].n[1]))
                           + plane->n.n[2] * ((vertices[2].n[0] - vertices[0].n[0]) * (vertices[1].n[1] - vertices[0].n[1])
                                                 - (vertices[2].n[1] - vertices[0].n[1]) * (vertices[1].n[0] - vertices[0].n[0]))
                           + plane->n.n[1] * ((vertices[2].n[2] - vertices[0].n[2]) * (vertices[1].n[0] - vertices[0].n[0])
                                                 - (vertices[2].n[0] - vertices[0].n[0]) * (vertices[1].n[2] - vertices[0].n[2]))) > 0.0f;

        side_count[front_facing]    = vertex_count;
        side_count[!front_facing]   = 0;
        side_vertices[front_facing] = vertices;

        on_node_designator = front_facing ? (node_index | 0x80000000) : (node_index & 0x7FFFFFFF);
    }
    else
    {
        real_plane3d negated_plane;
        negated_plane.n.n[0] = -plane->n.n[0];
        negated_plane.n.n[1] = -plane->n.n[1];
        negated_plane.n.n[2] = -plane->n.n[2];
        negated_plane.d    = -plane->d;

        side_count[0]    = convex_polygon3d_clip_to_plane(vertex_count, vertices, &negated_plane, 64,
                                                          back_clipped, NULL, epsilon, keep_coplanar);
        side_count[1]    = convex_polygon3d_clip_to_plane(vertex_count, vertices, plane, 64,
                                                          front_clipped, NULL, epsilon, keep_coplanar);
        side_vertices[0] = back_clipped;
        side_vertices[1] = front_clipped;
    }

    int leaf_count = 0;
    for (int side = 0; side < 2; ++side)
    {
        if (!side_count[side])
            continue;

        int child = node->child_indices[side];  /* [0]=back, [1]=front */
        if (child >= 0)
        {
            leaf_count += bsp3d_clip_polygon_to_leaves_recursive(bsp, child, on_node_designator,
                    side_vertices[side], side_count[side], epsilon, handler, user_data);
        }
        else if (child != -1)
        {
            if (handler)
                handler(side_vertices[side], side_count[side], child & 0x7FFFFFFF, on_node_designator, user_data);
            ++leaf_count;
        }
    }

    return leaf_count;
}
