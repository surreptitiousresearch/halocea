/* leaf_map_build_portal_from_leaves @0x8381B7F8 — build the portal polygon shared by two leaves across one BSP
 * node's splitting plane. Each leaf carries a list of 16-byte faces (first int = the node index the face lies
 * on); this finds the face in each leaf that sits on `node_index`, intersects their two 2D convex hulls
 * (convex_hull2d_intersect, up to 64 vertices), and if a non-degenerate polygon results allocates a portal
 * (leaf_map->portals) plus a portal designator in each leaf, links them, projects the 2D intersection back into
 * 3D through the node plane, and stores it as the portal's vertex list. A portal too small by area
 * (< 0.0025) or too sliver-like (sqrt(area)/perimeter < 0.01) is immediately closed again. On any allocation
 * failure it records the first error string in leaf_map_globals.
 *
 * DEVIATION: the decompiler mis-types the portal record as a bare tag_block (rendering plane_index/leaf0/leaf1
 * as ->count/->address/->definition); disasm (0x8381B9AC-B9E8) shows three full-word stores, so it is the
 * modeled leaf_map_portal {plane_index, leaf_index0, leaf_index1, vertices}. Leaves are map_leaf (24 bytes,
 * DB type: faces block + portal_designators block); faces are map_leaf_face {node_index, vertices tag_block}.
 *
 * DEVIATION: the decompiler's leaf address `16*n + 8*(n & 0x7FFFFFFF)` is exactly `24 * (n & 0x7FFFFFFF)` in
 * 32-bit arithmetic — 16*n differs from 16*(n & 0x7FFFFFFF) only by 16*2^31 = 2^35, which wraps to 0 — so it
 * is NOT the "24*n for non-negative n" identity: leaf_index1 arrives sign-bit-tagged from
 * leaf_map_build_portals_from_leaf and the masked index is the correct one for all inputs. Disasm
 * 0x8381B808-B828 confirms the shape: clrlwi m, n, 1 / add (m + 2n) / slwi 3. */

#include <stdint.h>
#include "headers/leaf_map.h"
#include "headers/leaf_map_portal.h"
#include "headers/map_leaf.h"
#include "headers/map_leaf_face.h"
#include "headers/bsp3d.h"
#include "headers/bsp3d_node.h"
#include "headers/tag_block.h"
#include "headers/real_point2d.h"
#include "headers/real_point3d.h"
#include "headers/real_plane3d.h"

#include "headers/real_vector3d.h"
#include "headers/tag_groups.h"
extern int16_t convex_hull2d_intersect(int16_t p_count, const real_point2d *p, int q_count, const real_point2d *q, int16_t maximum_count, real_point2d *result, float epsilon);
extern int16_t projection_from_vector3d(const real_vector3d *n);
extern real_point3d *project_point2d(const real_point2d *p2d, const real_plane3d *plane, int16_t projection, uint8_t sign, real_point3d *p3d);
extern float convex_hull2d_area(int16_t count, const real_point2d *points);
extern float convex_hull2d_perimeter(int16_t count, const real_point2d *points);
extern void leaf_map_close_portal(leaf_map *leaf_map, int portal_index);
extern float __fsqrts(float x);

/* first face index in `leaf` whose node index == node_index, or -1 */
static int16_t find_face_on_node(const map_leaf *leaf, int node_index)
{
    if ( leaf->faces.count <= 0 )
        return -1;
    const map_leaf_face *faces = (const map_leaf_face *)leaf->faces.address;
    int16_t face = 0;
    while ( faces[face].node_index != node_index )
    {
        ++face;
        if ( face >= leaf->faces.count )
            return -1;
    }
    return face;
}

void leaf_map_build_portal_from_leaves(leaf_map *leaf_map, int node_index, int leaf_index0, int leaf_index1)
{
    map_leaf *leaves = (map_leaf *)leaf_map->leaves.address;
    int masked_leaf0 = leaf_index0 & 0x7FFFFFFF;
    int masked_leaf1 = leaf_index1 & 0x7FFFFFFF;
    map_leaf *leaf0 = &leaves[masked_leaf0];
    map_leaf *leaf1 = &leaves[masked_leaf1];

    int16_t face0 = find_face_on_node(leaf0, node_index);
    int16_t face1 = find_face_on_node(leaf1, node_index);
    if ( face0 == -1 || face1 == -1 )
        return;

    map_leaf_face *face0_ptr = &((map_leaf_face *)leaf0->faces.address)[face0];
    map_leaf_face *face1_ptr = &((map_leaf_face *)leaf1->faces.address)[face1];

    real_point2d intersection[65];
    int vertex_count = convex_hull2d_intersect(face0_ptr->vertices.count,
            (const real_point2d *)face0_ptr->vertices.address, face1_ptr->vertices.count,
            (const real_point2d *)face1_ptr->vertices.address, 64, intersection, 0.00048828125);
    if ( vertex_count <= 0 )
        return;

    int portal_index = tag_block_add_element(&leaf_map->portals);
    int designator0 = tag_block_add_element(&leaf0->portal_designators);
    int designator1 = tag_block_add_element(&leaf1->portal_designators);
    if ( portal_index == -1 || designator0 == -1 || designator1 == -1 )
    {
        if ( !leaf_map_globals.__noop )
        {
            if ( portal_index == -1 )
                leaf_map_globals.__noop = "couldn't allocate leaf map portal";
            else
                leaf_map_globals.__noop = "couldn't allocate leaf map portal designator.";
        }
        return;
    }

    int plane_index = ((bsp3d_node *)leaf_map->bsp->nodes.address)[node_index].plane_index;
    /* recovered: (char *)portals.address + 24 * portal_index -> typed leaf_map_portal[] indexing */
    leaf_map_portal *portals = (leaf_map_portal *)leaf_map->portals.address;
    leaf_map_portal *portal = &portals[portal_index];
    const real_plane3d *plane = &((const real_plane3d *)leaf_map->bsp->planes.address)[plane_index];

    portal->plane_index = plane_index;
    int16_t projection = projection_from_vector3d(&plane->n);
    uint8_t sign = plane->n.n[projection] > 0.0f;
    portal->leaf_index0 = masked_leaf0;
    portal->leaf_index1 = masked_leaf1;
    ((int *)leaf0->portal_designators.address)[designator0] = portal_index;
    ((int *)leaf1->portal_designators.address)[designator1] = portal_index;

    if ( tag_block_resize(&portal->vertices, vertex_count) && vertex_count > 0 )
    {
        for ( int i = 0; i < vertex_count; i = (int16_t)(i + 1) )
            project_point2d(&intersection[i], plane, projection, sign,
                    (real_point3d *)portal->vertices.address + i);
    }

    float area = convex_hull2d_area(vertex_count, intersection);
    if ( area < 0.0024999999f
      || (__fsqrts(area) / convex_hull2d_perimeter(vertex_count, intersection)) < 0.0099999998f )
        leaf_map_close_portal(leaf_map, portal_index);
}
