/* collision_model_test_pill @0x837BBB58 — sweep a pill of `radius` along `vector` from `point` against an object's
 * collision model. For each node, selects the region permutation's collision BSP, transforms the pill into node
 * space (via the inverse node matrix, scaling the radius by the matrix scale), and runs collision_bsp_test_pill,
 * keeping the nearest hit. Records the struck node/region/permutation. Returns TRUE if any node was struck.
 *
 * Deviation: DB prototype is authoritative (5 params) — Hex-Rays split `result` into a phantom 5th arg plus the
 * real `a6`. Verified via prologue: instance=r3, point=r4, vector=r5, radius=f1 (does NOT reserve a GPR here),
 * result=r7. The per-node BSP test reuses &result->bsp_result as its scratch buffer, and result->bsp_result.t
 * carries the running best contact fraction (maximum_t) across nodes. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_matrix4x3.h"
#include "headers/collision_model_instance.h"
#include "headers/collision_node.h"
#include "headers/collision_bsp.h"
#include "headers/collision_bsp_test_pill_result.h"
#include "headers/collision_model_test_pill_result.h"

#include "headers/collision_bsp_test_pill_result.h"
extern uint8_t collision_bsp_test_pill(const collision_bsp *bsp, const real_point3d *point, const real_vector3d *vector, float radius, float maximum_t, collision_bsp_test_pill_result *result);
extern void matrix4x3_inverse(const real_matrix4x3 *matrix, real_matrix4x3 *result);
extern real_point3d *matrix4x3_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);
extern real_vector3d *matrix4x3_transform_vector(const real_matrix4x3 *matrix, const real_vector3d *vector, real_vector3d *result);

uint8_t collision_model_test_pill(const collision_model_instance *instance, const real_point3d *point,
                                          const real_vector3d *vector, float radius,
                                          collision_model_test_pill_result *result)
{
    int struck = 0;
    int16_t node_index = 0;

    result->bsp_result.t = 3.4028235e38;
    const collision_model *model = instance->model;
    if ( model->nodes.count > 0 )
    {
        int node = 0;
        do
        {
            const collision_node *node_def = &((const collision_node *)model->nodes.address)[node];
            int16_t region = node_def->region_index;
            if ( region != -1 )
            {
                int permutation = instance->region_permutation_indices[region];
                int permutation_count = node_def->bsps.count;
                if ( permutation_count > 0 )
                {
                    if ( permutation > permutation_count - 1 )
                        permutation = permutation_count - 1;
                    int16_t permutation_index = permutation;
                    const collision_bsp *bsp =
                        &((const collision_bsp *)node_def->bsps.address)[permutation_index];
                    if ( bsp->bsp3d.nodes.count > 0 )
                    {
                        real_matrix4x3 inverse;
                        real_point3d local_point;
                        real_vector3d local_vector;
                        matrix4x3_inverse(&instance->matrices[node], &inverse);
                        matrix4x3_transform_point(&inverse, point, &local_point);
                        matrix4x3_transform_vector(&inverse, vector, &local_vector);
                        if ( collision_bsp_test_pill(bsp, &local_point, &local_vector,
                                                     (inverse.scale * radius),
                                                     result->bsp_result.t, &result->bsp_result) )
                        {
                            result->node_index = node_index;
                            struck = 1;
                            result->region_index = node_def->region_index;
                            result->bsp_index = permutation_index;
                        }
                    }
                }
            }
            model = instance->model;
            node_index = (int16_t)(node + 1);
            node = node_index;
        }
        while ( node_index < model->nodes.count );
    }
    return struck;
}
