#include <stdint.h>
#include "headers/bsp3d.h"
#include "headers/real_point3d.h"

extern int bsp3d_clip_polygon_to_leaves_recursive(const bsp3d *bsp, int node_index, int on_node_designator, const real_point3d *vertices, int16_t vertex_count, float epsilon, void (*handler)(const real_point3d *, int16_t, int, int, void *), void *user_data);

/* DEFECT_FIXED 2026-08-03: the prior reconstruction typed `user_data` as a callback and passed it
 * as BOTH the handler and the data argument of the recursive call — reproducing a decompiler
 * conflation, not the binary. Disasm @0x837EA818-0x837EA82C proves the wrapper moves r8(handler)->r9
 * and r9(user_data)->r10 as distinct passthroughs, and the recursive callee never invokes r10
 * (user_data) — it only passes it as handler's 5th arg — so user_data is `void *`, not a funcptr. */
int bsp3d_clip_polygon_to_leaves(
        const bsp3d *bsp,
        int root_node_index,
        const real_point3d *vertices,
        int16_t vertex_count,
        float epsilon,
        void (*handler)(const real_point3d *, int16_t, int, int, void *),
        void *user_data)
{
    return bsp3d_clip_polygon_to_leaves_recursive(
               bsp, root_node_index, -1, vertices, vertex_count, epsilon,
               handler, user_data);
}
