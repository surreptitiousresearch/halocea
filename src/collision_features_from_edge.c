/* collision_features_from_edge @0x838049E0 — turn one collision-BSP edge into a swept-line collision feature.
 * Rejects the edge when it is a convex/coplanar boundary between its two surfaces (silhouette test on the two
 * plane normals vs the edge direction); otherwise transforms the edge segment by `matrix` (if any) and forwards
 * to collision_features_from_line.
 *
 * Deviation: the Hex-Rays output is float-GPR-skip scrambled and contains a spurious `v23 = "func __console__..."`
 * assignment from register reuse — dropped. True forwarded mapping (from disasm) is from_line(point, vector,
 * height, width, object_index, surface_index, surface.flags, surface.breakable_surface_index,
 * surface.material_index, features). Plane pointers use the masked plane index (the orientation sign bit is
 * shifted out by the *16 stride and is read separately for the silhouette test).
 *
 * Deviation: the two silhouette rejections are EXCLUSIVE, not sequential. 83804AE8 `bne cr6, loc_83804B40`
 * sends the opposite-facing case straight to the second cross block, so the same-facing block at 83804AEC
 * runs only when the signs match. Its reject path is 83804B34 `bgt cr6, loc_83804B8C` (cross > -eps =>
 * accept) falling into 83804B38 `cmplw cr6, r7, r8` / 83804B3C `beq cr6, loc_83804C00`: r7/r8 are the two
 * sign bytes and are unwritten by the block, so on this path they are provably equal and the beq to the
 * epilogue always fires. The fallthrough from 83804B3C into the second block is therefore DEAD, and reading
 * it as a live edge is what previously made this a sequential pair — which rejected every same-facing edge
 * whose cross was >= +eps, leaving only the near-coplanar band. Both blocks compute the same cross value
 * with different operand orderings; the binary genuinely duplicates it per arm, so both are kept. */

#include <stdint.h>
#include "headers/collision_bsp.h"
#include "headers/collision_surface.h"
#include "headers/collision_edge.h"
#include "headers/collision_vertex.h"
#include "headers/real_matrix4x3.h"
#include "headers/real_plane3d.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/collision_feature_list.h"

extern real_point3d *matrix4x3_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);
extern real_vector3d *matrix4x3_transform_vector(const real_matrix4x3 *matrix, const real_vector3d *vector, real_vector3d *result);
extern void collision_features_from_line(const real_point3d *point, const real_vector3d *vector, float height, float width, int object_index, int surface_index, uint8_t flags, uint8_t breakable_surface_index, int16_t material_index, collision_feature_list *features);

void collision_features_from_edge(const collision_bsp *bsp, int edge_index, const real_matrix4x3 *matrix,
                                  float height, float width, int object_index, collision_feature_list *features)
{
    const collision_surface *surfaces = (const collision_surface *)bsp->surfaces.address;
    const collision_edge *edge = &((const collision_edge *)bsp->edges.address)[edge_index];

    const collision_surface *left_surface = &surfaces[edge->surface_indices[0]];
    int left_plane = left_surface->plane_designator;
    int right_plane = surfaces[edge->surface_indices[1]].plane_designator;
    if ( left_plane == right_plane )
        return;

    const collision_vertex *vertices = (const collision_vertex *)bsp->vertices.address;
    const real_plane3d *planes = (const real_plane3d *)bsp->bsp3d.planes.address;
    const real_point3d *vertex0 = &vertices[edge->vertex_indices[0]].point;
    const real_point3d *vertex1 = &vertices[edge->vertex_indices[1]].point;

    int left_plane_index = left_plane & 0x7FFFFFFF;
    int right_plane_index = right_plane & 0x7FFFFFFF;
    char left_sign = (left_plane < 0);
    char right_sign = (right_plane < 0);

    real_vector3d edge_vector;
    edge_vector.n[0] = vertex1->n[0] - vertex0->n[0];
    edge_vector.n[1] = vertex1->n[1] - vertex0->n[1];
    edge_vector.n[2] = vertex1->n[2] - vertex0->n[2];

    const real_plane3d *left = &planes[left_plane_index];
    const real_plane3d *right = &planes[right_plane_index];

    if ( left_plane_index != right_plane_index )
    {
        if ( left_sign == right_sign )
        {
            float cross_same_facing = ((((right->n.n[0] * left->n.n[2]) - (left->n.n[0] * right->n.n[2])) * edge_vector.n[1])
                                 + (((left->n.n[1] * right->n.n[2]) - (left->n.n[2] * right->n.n[1])) * edge_vector.n[0])
                                 + ((left->n.n[0] * right->n.n[1]) - (right->n.n[0] * left->n.n[1])) * edge_vector.n[2]);
            if ( cross_same_facing <= -0.000099999997 )
                return;
        }
        else
        {
            float cross_opposite_facing = ((((left->n.n[2] * right->n.n[0]) - (right->n.n[2] * left->n.n[0])) * edge_vector.n[1])
                                 + (((left->n.n[1] * right->n.n[2]) - (left->n.n[2] * right->n.n[1])) * edge_vector.n[0])
                                 + ((right->n.n[1] * left->n.n[0]) - (left->n.n[1] * right->n.n[0])) * edge_vector.n[2]);
            if ( cross_opposite_facing >= 0.000099999997 )
                return;
        }
    }

    int surface_index = (object_index == -1) ? edge->surface_indices[0] : -1;

    real_vector3d transformed_vector;
    real_point3d transformed_point;
    const real_vector3d *vector;
    const real_point3d *point;
    if ( matrix )
    {
        vector = matrix4x3_transform_vector(matrix, &edge_vector, &transformed_vector);
        point = matrix4x3_transform_point(matrix, vertex0, &transformed_point);
    }
    else
    {
        vector = &edge_vector;
        point = vertex0;
    }

    collision_features_from_line(point, vector, height, width, object_index, surface_index,
                                 left_surface->flags, left_surface->breakable_surface_index,
                                 left_surface->material_index, features);
}
