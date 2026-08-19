/* leaf_map_build_leaf_face_for_leaf_on_node @0x8381BC50 — build the 2D face polygon a BSP leaf projects onto one
 * of its bounding node planes and store it in the leaf. Starts from the default (unbounded) face in the node
 * plane's own 2D space, then walks the current BSP traversal node stack and, for every other node on the stack,
 * intersects that node's plane (flipped when the stack entry is negative, i.e. the traversal took the back side)
 * with the leaf plane to get a clip line and clips the running polygon against it. If anything survives, a new
 * face element is appended to the leaf's face block: its node designator plus the clipped vertex list.
 *
 * The plane for a node reference is nodes[node & 0x7FFFFFFF].plane_index -> planes[]. The decompiler/disasm
 * express the node-array subscript as `2*node + (node & 0x7FFFFFFF)`, which is congruent to 3*(node & 0x7FFFFFFF)
 * mod 2^32 (nodes are 12-byte {plane_index, back, front}); reproduced verbatim in leaf_map_node_plane.
 *
 * DEVIATION: Hex-Rays modeled the polygon buffer with HIWORD/SHIWORD punning of a _DWORD[130]; it is a
 * convex_polygon2d (count @0x00, points @0x04) — the count sits in the first halfword (big-endian HIWORD),
 * confirmed by `lhz r3, 0(buffer)` @0x8381BCAC. The __noop assignments are the release error-message-sink idiom
 * (same as elsewhere in the leaf_map globals). */

#include <stdint.h>
#include "headers/leaf_map.h"
#include "headers/map_leaf.h"
#include "headers/map_leaf_face.h"
#include "headers/bsp3d.h"
#include "headers/convex_polygon2d.h"
#include "headers/real_plane3d.h"
#include "headers/real_plane2d.h"
#include "headers/real_point2d.h"
#include <string.h>
#include "headers/blam_data_globals.h"


#include "headers/real_point2d.h"
#include "headers/tag_groups.h"
extern int16_t intersect_planes3d(const real_plane3d *plane, const real_plane3d *reference_plane, real_plane2d *result);
extern int16_t convex_polygon2d_clip_to_plane(int16_t count, const real_point2d *points, const real_plane2d *plane, int16_t maximum_count, real_point2d *result, uint32_t *clip_flags, uint8_t *clipped, float epsilon);

static const real_plane3d *leaf_map_node_plane(const leaf_map *leaf_map, int node)
{
    /* unresolved: bsp3d_node stride 3 (12 bytes), plane_index field 0 — but the folded index
     * 2*node + (node & 0x7FFFFFFF) only reduces to 3*node for non-negative node; leaf_map node
     * references can carry the sign bit, so the raw arithmetic is preserved rather than retyped. */
    int plane_index = ((const int *)leaf_map->bsp->nodes.address)[2 * node + (node & 0x7FFFFFFF)];
    return &((const real_plane3d *)leaf_map->bsp->planes.address)[plane_index];
}

void leaf_map_build_leaf_face_for_leaf_on_node(leaf_map *leaf_map, int leaf_index, int node)
{
    int face_node = node & 0x7FFFFFFF;
    const real_plane3d *leaf_plane = leaf_map_node_plane(leaf_map, node);

    convex_polygon2d face;
    memcpy(&face, &default_face, sizeof(face));

    int16_t point_count = face.point_count;
    int stack_count = leaf_map_globals.node_stack_count;
    if ( stack_count > 0 )
    {
        int i = 0;
        do
        {
            if ( !point_count )
                break;
            int stack_node = leaf_map_globals.node_stack[stack_count - (int16_t)i - 1];
            if ( stack_node != node )
            {
                real_plane3d clip_plane = *leaf_map_node_plane(leaf_map, stack_node);
                if ( stack_node < 0 )
                {
                    clip_plane.n.n[0] = -clip_plane.n.n[0];
                    clip_plane.n.n[1] = -clip_plane.n.n[1];
                    clip_plane.n.n[2] = -clip_plane.n.n[2];
                    clip_plane.d = -clip_plane.d;
                }

                real_plane2d clip_line;
                int16_t result = intersect_planes3d(&clip_plane, leaf_plane, &clip_line);
                if ( result == 1 )
                    point_count = convex_polygon2d_clip_to_plane(face.point_count, face.points, &clip_line,
                            64, face.points, nullptr, nullptr, 0.00024414062f);
                else
                    point_count = (result == 0) ? 0 : face.point_count;
                face.point_count = point_count;
            }
            i = (int16_t)(i + 1);
            stack_count = leaf_map_globals.node_stack_count;
        }
        while ( i < leaf_map_globals.node_stack_count );
    }

    if ( point_count )
    {
        /* leaf element stride is 24 bytes (map_leaf); the subscript expression
         * 16*leaf_index + 8*(leaf_index & 0x7FFFFFFF) is 24*leaf_index for non-negative indices. */
        tag_block *faces = &((map_leaf *)leaf_map->leaves.address)[leaf_index].faces;
        int16_t face_element = tag_block_add_element(faces);
        if ( face_element == -1 )
        {
            if ( !leaf_map_globals.__noop )
                leaf_map_globals.__noop = "couldn't allocate leaf face.";
        }
        else
        {
            map_leaf_face *leaf_face = &((map_leaf_face *)faces->address)[face_element];
            leaf_face->node_index = face_node;
            if ( tag_block_resize(&leaf_face->vertices, face.point_count) )
                memcpy(leaf_face->vertices.address, face.points, 8 * face.point_count);
            else if ( !leaf_map_globals.__noop )
                leaf_map_globals.__noop = "couldn't allocate leaf vertices.";
        }
    }
}
