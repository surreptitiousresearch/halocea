/* collision_test_sphere @0x83772DF8 — tests whether a sphere at `center` (ignoring one object) collides with
 * the world: first a coarse point-in-bsp3d test (treats "outside the bsp" as a collision), then a precise
 * breakable-surface-aware sphere test against the collision bsp. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/collision_bsp.h"
#include "headers/collision_bsp_test_sphere_result.h"
#include "headers/bsp3d.h"
#include "headers/blam_data_globals.h"


#include "headers/collision_bsp.h"
extern int bsp3d_test_point(const bsp3d *bsp, int node_index, const real_point3d *point);
extern const uint8_t *breakable_surface_flags_get(void);
extern uint8_t collision_bsp_test_sphere(const collision_bsp *bsp, int16_t breakable_surface_count, const uint8_t *breakable_surface_flags, const real_point3d *center, float radius, collision_bsp_test_sphere_result *result);

uint8_t collision_test_sphere(const real_point3d *center, float radius, int ignore_object_index)
{
    if ( bsp3d_test_point(global_bsp3d, 0, center) == -1 )
        return 1;

    const uint8_t *breakable_surface_flags = breakable_surface_flags_get();
    collision_bsp_test_sphere_result result;

    if ( collision_bsp_test_sphere(global_collision_bsp, 256, breakable_surface_flags, center, radius, &result) == 0 )
        return 1;

    return 0;
}
