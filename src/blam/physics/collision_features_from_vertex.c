/* collision_features_from_vertex @0x83804918 — turn one collision-BSP vertex into point collision features.
 * Resolves the vertex position (optionally transformed by `matrix`), derives the owning surface from the vertex's
 * first edge, and forwards to collision_features_from_point.
 *
 * Deviation: float-GPR-skip scrambles the Hex-Rays parameter/argument names. True register map (from the
 * prologue) is r3=bsp, r4=vertex_index, r5=matrix, f1=height, f2=width, r8=object_index, r9=features; and the
 * forwarded call maps to from_point as (point, height, width, object_index, surface_index, surface.flags,
 * surface.breakable_surface_index, surface.material_index, features). */

#include <stdint.h>
#include "headers/collision_bsp.h"
#include "headers/collision_surface.h"
#include "headers/collision_edge.h"
#include "headers/collision_vertex.h"
#include "headers/real_matrix4x3.h"
#include "headers/real_point3d.h"
#include "headers/collision_feature_list.h"

extern real_point3d *matrix4x3_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);
extern void collision_features_from_point(const real_point3d *point, float height, float width, int object_index, int surface_index, uint8_t flags, uint8_t breakable_surface_index, int16_t material_index, collision_feature_list *features);

void collision_features_from_vertex(const collision_bsp *bsp, int vertex_index, const real_matrix4x3 *matrix,
                                    float height, float width, int object_index, collision_feature_list *features)
{
    const collision_vertex *vertex = &((const collision_vertex *)bsp->vertices.address)[vertex_index];
    int first_edge = vertex->first_edge_index;
    int surface_index = ((const collision_edge *)bsp->edges.address)[first_edge].surface_indices[0];
    const collision_surface *surface = &((const collision_surface *)bsp->surfaces.address)[surface_index];

    int reported_surface_index = (object_index == -1) ? surface_index : -1;

    real_point3d transformed;
    const real_point3d *point = matrix ? matrix4x3_transform_point(matrix, &vertex->point, &transformed) : &vertex->point;

    collision_features_from_point(point, height, width, object_index, reported_surface_index,
                                  surface->flags, surface->breakable_surface_index,
                                  surface->material_index, features);
}
