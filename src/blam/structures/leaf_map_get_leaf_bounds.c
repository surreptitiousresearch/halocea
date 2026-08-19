/* leaf_map_get_leaf_bounds @0x8381AA68 — compute the axis-aligned bounding box of one leaf_map leaf and
 * return its center plus bounding-sphere radius (half the AABB diagonal). The bounds accumulate over the
 * leaf's geometry, chosen by which representation the leaf carries:
 *   - if the leaf has portal designators: union the vertices of every referenced portal polygon
 *     (real_point3d, already in world space);
 *   - else if the leaf has faces: for each face, reconstruct each 2D projected polygon vertex back to 3D
 *     using the face's BSP node plane (dominant-axis unprojection), and union those;
 *   - else (empty leaf): leave the bounds at the null-rectangle default.
 * The AABB is seeded from global_null_rectangle3d (an empty box) so the first real vertex initializes it.
 *
 * The 2D->3D unprojection mirrors the collision-surface siblings (e.g. collision_surface_find_closest_
 * point2d.c): global_projection3d_mappings[dominant_axis][normal_sign] selects the (u_axis, v_axis)
 * pair, and the third coordinate is solved from the plane equation. */

#include <stdint.h>
#include "headers/leaf_map.h"
#include "headers/map_leaf.h"
#include "headers/map_leaf_face.h"
#include "headers/leaf_map_portal.h"
#include "headers/bsp3d.h"
#include "headers/bsp3d_node.h"
#include "headers/real_plane3d.h"
#include "headers/real_point3d.h"
#include "headers/real_point2d.h"
#include "headers/real_rectangle3d.h"
#include "headers/blam_data_globals.h"

/* pointer, not a value: disasm 0x8381AA90 loads it with lwz, then copies through it (-> private_null_rectangle) */
extern float sqrtf(float x);
extern float fabsf(float x);

void leaf_map_get_leaf_bounds(const leaf_map *leaf_map, int leaf_index,
                              real_point3d *bounding_sphere_center, float *bounding_sphere_radius)
{
    /* recovered: (char *)leaves.address + 16*n + 8*(n & 0x7FFFFFFF) -> map_leaf[] indexing (24-byte stride) */
    map_leaf *leaf = &((map_leaf *)leaf_map->leaves.address)[leaf_index];

    float min_x = global_null_rectangle3d->x0, max_x = global_null_rectangle3d->x1;
    float min_y = global_null_rectangle3d->y0, max_y = global_null_rectangle3d->y1;
    float min_z = global_null_rectangle3d->z0, max_z = global_null_rectangle3d->z1;

    if ( leaf->portal_designators.count )
    {
        const int *portal_designators = (const int *)leaf->portal_designators.address;
        const leaf_map_portal *portals = (const leaf_map_portal *)leaf_map->portals.address;
        for ( int designator = 0; designator < leaf->portal_designators.count; ++designator )
        {
            /* recovered: (char *)portals + 24 * idx -> typed leaf_map_portal[] indexing */
            const leaf_map_portal *portal =
                &portals[portal_designators[designator] & 0x7FFFFFFF];
            const real_point3d *vertices = (const real_point3d *)portal->vertices.address;
            for ( int i = 0; i < portal->vertices.count; ++i )
            {
                float x = vertices[i].n[0];
                float y = vertices[i].n[1];
                float z = vertices[i].n[2];
                if ( x < min_x ) min_x = x;
                if ( x > max_x ) max_x = x;
                if ( y < min_y ) min_y = y;
                if ( y > max_y ) max_y = y;
                if ( z < min_z ) min_z = z;
                if ( z > max_z ) max_z = z;
            }
        }
    }
    else if ( leaf->faces.count > 0 )
    {
        const map_leaf_face *faces = (const map_leaf_face *)leaf->faces.address;
        const bsp3d_node *nodes = (const bsp3d_node *)leaf_map->bsp->nodes.address;
        const real_plane3d *planes = (const real_plane3d *)leaf_map->bsp->planes.address;

        for ( int face_index = 0; face_index < leaf->faces.count; ++face_index )
        {
            const map_leaf_face *face = &faces[face_index];
            const real_plane3d *plane =
                &planes[nodes[face->node_index].plane_index];
            const float *normal = plane->n.n;

            /* dominant axis = component of the plane normal with the largest magnitude */
            float abs_x = fabsf(normal[0]);
            float abs_y = fabsf(normal[1]);
            float abs_z = fabsf(normal[2]);
            int axis;
            if ( abs_z < abs_y || abs_z < abs_x )
                axis = (abs_y >= abs_x) ? 1 : 0;
            else
                axis = 2;

            float normal_axis = normal[axis];
            int axis_sign = (normal_axis > 0.0f);
            int u_axis = global_projection3d_mappings[axis][axis_sign][0];
            int v_axis = global_projection3d_mappings[axis][axis_sign][1];

            const real_point2d *vertices = (const real_point2d *)face->vertices.address;
            for ( int i = 0; i < face->vertices.count; ++i )
            {
                float u = vertices[i].n[0];
                float v = vertices[i].n[1];
                real_point3d point;
                point.n[u_axis] = u;
                point.n[v_axis] = v;
                if ( fabsf(normal_axis) >= 0.0001f )
                    point.n[axis] = (plane->d - normal[u_axis] * u - normal[v_axis] * v) / normal_axis;
                else
                    point.n[axis] = 0.0f;

                if ( point.n[0] < min_x ) min_x = point.n[0];
                if ( point.n[0] > max_x ) max_x = point.n[0];
                if ( point.n[1] < min_y ) min_y = point.n[1];
                if ( point.n[1] > max_y ) max_y = point.n[1];
                if ( point.n[2] < min_z ) min_z = point.n[2];
                if ( point.n[2] > max_z ) max_z = point.n[2];
            }
        }
    }

    float center_x = (max_x + min_x) * 0.5f;
    float center_y = (max_y + min_y) * 0.5f;
    float center_z = (max_z + min_z) * 0.5f;
    bounding_sphere_center->n[0] = center_x;
    bounding_sphere_center->n[1] = center_y;
    bounding_sphere_center->n[2] = center_z;

    float half_extent_x = max_x - center_x;
    float half_extent_y = max_y - center_y;
    float half_extent_z = max_z - center_z;
    *bounding_sphere_radius = sqrtf(half_extent_y * half_extent_y
                                    + (half_extent_x * half_extent_x + half_extent_z * half_extent_z));
}
