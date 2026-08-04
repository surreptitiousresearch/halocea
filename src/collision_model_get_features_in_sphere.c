/* collision_model_get_features_in_sphere @0x837BBCC8 — gather collision features from every node of an object's
 * collision model that the query sphere overlaps. For each node it selects the region/permutation BSP, transforms
 * the query centre into that node's local space, runs collision_bsp_test_sphere, and converts the hits to
 * features via collision_bsp_get_features_in_sphere. Returns whether any node produced features.
 *
 * Deviation: the Hex-Rays output dropped the `result = &test_result` and the object_index/features call
 * arguments. True mapping (from disasm): collision_bsp_test_sphere(bsp, 0, NULL, &local_centre,
 * inverse.scale*radius, &test_result); collision_bsp_get_features_in_sphere(bsp, &test_result, matrix, height,
 * width, instance->object_index, features). */

#include <stdint.h>
#include "headers/collision_model_instance.h"
#include "headers/collision_node.h"
#include "headers/collision_bsp.h"
#include "headers/collision_bsp_test_sphere_result.h"
#include "headers/real_point3d.h"
#include "headers/real_matrix4x3.h"
#include "headers/collision_feature_list.h"

extern void matrix4x3_inverse(const real_matrix4x3 *matrix, real_matrix4x3 *result);
extern real_point3d *matrix4x3_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);
extern uint8_t collision_bsp_test_sphere(const collision_bsp *bsp, int16_t breakable_surface_count, const uint8_t *breakable_surface_flags, const real_point3d *center, float radius, collision_bsp_test_sphere_result *result);
extern void collision_bsp_get_features_in_sphere(const collision_bsp *bsp, const collision_bsp_test_sphere_result *result, const real_matrix4x3 *matrix, float height, float width, int object_index, collision_feature_list *features);

uint8_t collision_model_get_features_in_sphere(const collision_model_instance *instance, const real_point3d *center,
                                           float radius, float height, float width,
                                           collision_feature_list *features)
{
    int produced = 0;

    for ( int node_index = 0; node_index < instance->model->nodes.count; node_index = (__int16)(node_index + 1) )
    {
        const collision_node *node = &((const collision_node *)instance->model->nodes.address)[node_index];
        __int16 region_index = node->region_index;
        if ( region_index == -1 )
            continue;

        int permutation = instance->region_permutation_indices[region_index];
        int bsp_count = node->bsps.count;
        if ( bsp_count <= 0 )
            continue;

        int last_bsp = bsp_count - 1;
        if ( permutation > last_bsp )
            permutation = (__int16)last_bsp;

        const collision_bsp *bsp = &((const collision_bsp *)node->bsps.address)[(__int16)permutation];
        if ( bsp->bsp3d.nodes.count <= 0 )
            continue;

        const real_matrix4x3 *matrix = &instance->matrices[node_index];
        real_matrix4x3 inverse;
        matrix4x3_inverse(matrix, &inverse);
        real_point3d local_center;
        matrix4x3_transform_point(&inverse, center, &local_center);

        collision_bsp_test_sphere_result test_result;
        if ( collision_bsp_test_sphere(bsp, 0, (const unsigned char *)0, &local_center,
                                       (inverse.scale * radius), &test_result) )
        {
            collision_bsp_get_features_in_sphere(bsp, &test_result, matrix, height, width,
                                                 instance->object_index, features);
            produced = 1;
        }
    }

    return produced;
}
