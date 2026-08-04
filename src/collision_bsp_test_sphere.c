/* collision_bsp_test_sphere @0x837E24B0 — gather the collision-BSP surfaces/edges/vertices/leaves overlapping a
 * query sphere by recursing the BSP from the root. Returns TRUE if any surface or edge was collected.
 *
 * Deviation: Hex-Rays split `result` into a phantom param plus the real a7; the DB call sites pass one result. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/test_sphere_data.h"

extern void bsp3d_test_sphere_recursive(test_sphere_data *data, int child_index);

uint8_t collision_bsp_test_sphere(const collision_bsp *bsp, int16_t breakable_surface_count,
                              const uint8_t *breakable_surface_flags, const real_point3d *center,
                              float radius, collision_bsp_test_sphere_result *result)
{
    test_sphere_data data;
    data.bsp = bsp;
    data.breakable_surface_count = breakable_surface_count;
    data.radius = radius;
    result->leaf_count = 0;
    result->surface_count = 0;
    result->edge_count = 0;
    result->vertex_count = 0;
    data.breakable_surface_flags = breakable_surface_flags;
    data.center = center;
    data.result = result;
    data.stack_depth = 0;
    bsp3d_test_sphere_recursive(&data, 0);
    return result->surface_count > 0 || result->edge_count > 0;
}
