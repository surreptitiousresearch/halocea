/* collision_model_test_sphere @0x837BB8F8 — SHIPPED STUB: walks every node of the collision model,
 * resolves each node's region/permutation collision BSP, and (when that BSP has content) computes the
 * node's inverse matrix and transforms `center` into node-local space — but never runs an actual
 * sphere-vs-BSP test with the result, and `radius` is entirely unused. Always returns 0 (no hit).
 * Disasm-verified: no call past matrix4x3_transform_point exists in the loop body; the function
 * unconditionally `li r3,0` before returning. Reproduced verbatim, dead work included. */

#include "headers/real_point3d.h"
#include "headers/real_matrix4x3.h"
#include "headers/collision_model_instance.h"
#include "headers/collision_bsp.h"
#include "headers/collision_node.h"

extern void matrix4x3_inverse(const real_matrix4x3 *matrix, real_matrix4x3 *result);
extern real_point3d *matrix4x3_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);

int collision_model_test_sphere(const collision_model_instance *instance, const real_point3d *center, float radius)
{
    const collision_model *model = instance->model;
    if ( model->nodes.count > 0 )
    {
        int node = 0;
        do
        {
            collision_node *node_def = &((collision_node *)model->nodes.address)[node];
            __int16 region = node_def->region_index;
            if ( region != -1 )
            {
                int permutation = instance->region_permutation_indices[region];
                int permutation_count = node_def->bsps.count;
                if ( permutation_count > 0 )
                {
                    if ( permutation > permutation_count - 1 )
                        permutation = permutation_count - 1;
                    /* recovered: 96*permutation + base -> collision_bsp array indexing (96 = sizeof) */
                    const collision_bsp *bsp =
                        &((const collision_bsp *)node_def->bsps.address)[(__int16)permutation];
                    if ( bsp->bsp3d.nodes.count > 0 )
                    {
                        real_matrix4x3 inverse;
                        real_point3d local_center;
                        matrix4x3_inverse(&instance->matrices[node], &inverse);
                        matrix4x3_transform_point(&inverse, center, &local_center);
                        /* local_center, inverse.scale * radius: never used — shipped incomplete */
                    }
                }
            }
            model = instance->model;
            node = (__int16)(node + 1);
        }
        while ( node < model->nodes.count );
    }
    return 0;
}
