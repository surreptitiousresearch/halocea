/* collision_bsp_get_features_in_sphere @0x83804D88 — convert the overlapping structure/collision-BSP vertices,
 * edges and surfaces collected by collision_bsp_test_sphere into sphere/cylinder/prism collision features (in the
 * given object's space), appending them to `features`. */

#include "headers/real_matrix4x3.h"
#include "headers/collision_bsp.h"
#include "headers/collision_bsp_test_sphere_result.h"
#include "headers/collision_feature_list.h"

extern void collision_features_from_vertex(const collision_bsp *bsp, int vertex_index, const real_matrix4x3 *matrix, float height, float width, int object_index, collision_feature_list *features);
extern void collision_features_from_edge(const collision_bsp *bsp, int edge_index, const real_matrix4x3 *matrix, float height, float width, int object_index, collision_feature_list *features);
extern void collision_features_from_surface(const collision_bsp *bsp, int surface_index, const real_matrix4x3 *matrix, float height, float width, int object_index, collision_feature_list *features);

void collision_bsp_get_features_in_sphere(const collision_bsp *bsp,
                                          const collision_bsp_test_sphere_result *result,
                                          const real_matrix4x3 *matrix, float height, float width,
                                          int object_index, collision_feature_list *features)
{
    for ( int i = 0; i < result->vertex_count; ++i )
        collision_features_from_vertex(bsp, result->vertex_indices[i], matrix, height, width, object_index, features);

    for ( int i = 0; i < result->edge_count; ++i )
        collision_features_from_edge(bsp, result->edge_indices[i], matrix, height, width, object_index, features);

    for ( int i = 0; i < result->surface_count; ++i )
        collision_features_from_surface(bsp, result->surface_indices[i], matrix, height, width, object_index, features);
}
