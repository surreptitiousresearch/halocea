/* leaf_face_get_vertex3d @0x8381B428 — reconstruct the 3D position of one of a leaf face's projected 2D
 * vertices: looks up the face's BSP splitting plane, picks the projection axis with the largest-magnitude
 * normal component (preferring z, then y-vs-x), and lifts the 2D vertex back onto that plane via
 * project_point2d. */

#include <stdint.h>
#include "headers/leaf_map.h"
#include "headers/bsp3d.h"
#include "headers/bsp3d_node.h"
#include "headers/map_leaf_face.h"
#include "headers/real_plane3d.h"
#include "headers/real_point2d.h"
#include "headers/real_point3d.h"

extern double __fabs(double x);
extern real_point3d *project_point2d(const real_point2d *p2d, const real_plane3d *plane, int16_t projection, uint8_t sign, real_point3d *p3d);

void leaf_face_get_vertex3d(const leaf_map *map, map_leaf_face *face, int16_t vertex_index, real_point3d *result)
{
    bsp3d_node *node = (bsp3d_node *)map->bsp->nodes.address + face->node_index;
    const real_plane3d *plane = &((const real_plane3d *)map->bsp->planes.address)[node->plane_index];

    float abs_x = (float)__fabs(plane->n.n[0]);
    float abs_y = (float)__fabs(plane->n.n[1]);
    float abs_z = (float)__fabs(plane->n.n[2]);

    int16_t projection;
    if ( abs_z < abs_y || abs_z < abs_x )
        projection = abs_y >= abs_x;
    else
        projection = 2;

    const real_point2d *vertex = (const real_point2d *)face->vertices.address + vertex_index;

    project_point2d(vertex, plane, projection, plane->n.n[projection] > 0.0f, result);
}
