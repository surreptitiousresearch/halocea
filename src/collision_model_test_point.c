/* collision_model_test_point @0x837BB7F0 — test whether a point lies inside an object's collision model. For each
 * model node with a region, selects the active permutation's collision BSP, transforms the point into node space,
 * and tests it against that BSP. Returns 1 as soon as the point is found inside any node's geometry. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_matrix4x3.h"
#include "headers/collision_model_instance.h"
#include "headers/collision_node.h"
#include "headers/collision_bsp.h"
#include "headers/bsp3d.h"

extern real_point3d *matrix4x3_inverse_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);
extern int bsp3d_test_point(const bsp3d *bsp, int node_index, const real_point3d *point);

uint8_t collision_model_test_point(const collision_model_instance *instance, const real_point3d *point)
{
    const collision_model *model = instance->model;
    if ( model->nodes.count <= 0 )
        return 0;

    for ( int node = 0; ; node = (int16_t)(node + 1) )
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
                const bsp3d *bsp = &((const collision_bsp *)node_def->bsps.address)[(int16_t)permutation].bsp3d;
                if ( bsp->nodes.count > 0 )
                {
                    real_point3d local;
                    matrix4x3_inverse_transform_point(&instance->matrices[node], point, &local);
                    if ( bsp3d_test_point(bsp, 0, &local) == -1 )
                        return 1;
                }
            }
        }
        model = instance->model;
        if ( (int16_t)(node + 1) >= model->nodes.count )
            return 0;
    }
}
