/* collision_test_pill @0x83773B80 — sweep a vertical pill (capsule) of the given radius along `vector` from
 * `point` against the structure BSP, writing the first contact into `collision`. Fills the surface plane/material
 * when flag 0x20 (return-surface) is set, records the start/end BSP clusters from the leaf list crossed, and
 * computes the final contact point and its location. Returns TRUE when a surface was struck.
 *
 * Deviation: DB prototype is authoritative — Hex-Rays mis-modelled the float `radius` (GPR-skip) and split
 * `collision` off as a phantom trailing arg. Verified via prologue: flags=r3, point=r4, vector=r5, radius=f1
 * (reserves r6), ignore_object_index=r7, collision=r8. ignore_object_index is unused by the BSP-only path. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/location.h"
#include "headers/collision_result.h"
#include "headers/collision_bsp_test_pill_result.h"
#include "headers/collision_bsp.h"
#include "headers/structure_bsp.h"
#include "headers/structure_leaf.h"
#include "headers/collision_result_type.h"
#include "headers/collision_test_flags.h"
#include "headers/blam_data_globals.h"


#include "headers/collision_bsp.h"
extern uint8_t collision_bsp_test_pill(const collision_bsp *bsp, const real_point3d *point, const real_vector3d *vector, float radius, float maximum_t, collision_bsp_test_pill_result *result);
extern void scenario_location_from_point(location *location, const real_point3d *point);

static int16_t leaf_cluster(int leaf_index)
{
    if ( leaf_index == -1 )
        return -1;
    return ((structure_leaf *)global_structure_bsp->leaves.address)[leaf_index].cluster_index;
}

uint8_t collision_test_pill(unsigned int flags, const real_point3d *point, const real_vector3d *vector,
                            float radius, int ignore_object_index, collision_result *collision)
{
    collision_bsp_test_pill_result bsp_result;
    int struck = 0;

    collision->type = -1;
    collision->t = 3.4028235e38;
    if ( collision_bsp_test_pill(global_collision_bsp, point, vector, radius, 3.4028235e38, &bsp_result) )
    {
        collision->t = bsp_result.t;
        if ( (flags & (1u << _collision_test_structure_bit)) != 0 )
        {
            collision->plane_designator = -1;
            collision->type = collision_result_structure;
            struck = 1;
            collision->flags = 0;
            collision->material_type = bsp_result.material_index;
            collision->material_index = bsp_result.material_index;
            collision->surface_index = bsp_result.surface_index;
            collision->plane = bsp_result.plane;
            collision->breakable_surface_index = 0;
        }
    }

    if ( bsp_result.leaf_count > 0 )
    {
        int first_leaf = bsp_result.leaf_indices[0];
        collision->start_location.leaf_index = first_leaf;
        collision->start_location.cluster_index = leaf_cluster(first_leaf);
        int last_leaf = bsp_result.leaf_indices[bsp_result.leaf_count - 1];
        collision->location.leaf_index = last_leaf;
        collision->location.cluster_index = leaf_cluster(last_leaf);
    }

    if ( !struck )
        collision->t = 1.0;
    float t = collision->t;
    collision->point.n[0] = (vector->n[0] * collision->t) + point->n[0];
    collision->point.n[1] = (vector->n[1] * t) + point->n[1];
    collision->point.n[2] = (vector->n[2] * t) + point->n[2];
    scenario_location_from_point(&collision->location, &collision->point);
    return struck;
}
