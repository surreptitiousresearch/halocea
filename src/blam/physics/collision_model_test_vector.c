/* collision_model_test_vector @ 0x837BB9E8 — test a ray against an object's collision model. For each
 * model node it selects the active region permutation's collision BSP, brings the ray into that
 * node's local space (via the inverse node matrix), and runs the recursive BSP vector test, keeping
 * the nearest hit and recording which node/region/bsp produced it. Returns 1 if any node was hit.
 *
 * The region permutation index selects the bsp, clamped to the available count. */

#include <stdint.h>
#include "headers/collision_model_instance.h"
#include "headers/collision_node.h"
#include "headers/collision_model_test_vector_result.h"
#include "headers/collision_bsp.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_matrix4x3.h"

#include "headers/collision_bsp_test_vector_result.h"
extern void matrix4x3_inverse(const real_matrix4x3 *matrix, real_matrix4x3 *result);
extern real_point3d *matrix4x3_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);
extern real_vector3d *matrix4x3_transform_vector(const real_matrix4x3 *matrix, const real_vector3d *vector, real_vector3d *result);
extern uint8_t collision_bsp_test_vector(unsigned int flags, const struct collision_bsp *bsp, int16_t breakable_surface_count, const uint8_t *breakable_surface_flags, const real_point3d *point, const real_vector3d *vector, float maximum_t, collision_bsp_test_vector_result *result);

uint8_t collision_model_test_vector(const collision_model_instance *instance, unsigned int flags,
                                const real_point3d *point, const real_vector3d *vector,
                                collision_model_test_vector_result *result)
{
    const collision_model *model = instance->model;
    int found = 0;
    int node_index;

    result->bsp_result.t = 3.4028235e38f;

    for ( node_index = 0; node_index < model->nodes.count; ++node_index )
    {
        const collision_node *node = &((const collision_node *)model->nodes.address)[node_index];
        int16_t region_index = node->region_index;
        int bsp_count;

        if ( region_index != -1 && (bsp_count = node->bsps.count) > 0 )
        {
            int permutation = instance->region_permutation_indices[region_index];
            int16_t bsp_index;
            const collision_bsp *bsp;

            if ( permutation > bsp_count - 1 )
                permutation = bsp_count - 1;
            bsp_index = (int16_t)permutation;
            bsp = &((const collision_bsp *)node->bsps.address)[bsp_index];

            if ( bsp->bsp3d.nodes.count > 0 )
            {
                real_matrix4x3 inverse;
                real_point3d local_point;
                real_vector3d local_vector;

                matrix4x3_inverse(&instance->matrices[node_index], &inverse);
                matrix4x3_transform_point(&inverse, point, &local_point);
                matrix4x3_transform_vector(&inverse, vector, &local_vector);

                if ( collision_bsp_test_vector(flags, bsp, 0, 0, &local_point, &local_vector,
                                               result->bsp_result.t, &result->bsp_result) )
                {
                    result->node_index = (int16_t)node_index;
                    found = 1;
                    result->region_index = node->region_index;
                    result->bsp_index = bsp_index;
                }
            }
        }
        model = instance->model;
    }

    return found;
}
