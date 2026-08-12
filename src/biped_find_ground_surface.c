/* biped_find_ground_surface @0x837AD1C8 — casts a ray from just above a biped's origin along a given
 * direction and reports the collision-BSP surface it hits. Returns the hit surface index (or -1 if
 * nothing was hit); optionally outputs the intersection point and the hit surface's plane normal.
 *
 * DEVIATION: the leading `float ground_distance` reserves its GPR slot (value in f1), so the
 * decompiler's argument names are shifted; mapped here from the disassembly — direction is r5,
 * surface_point is r6, surface_normal is r7 (there is no sixth argument). */

#include <stdint.h>
#include "headers/collision_bsp.h"
#include "headers/collision_bsp_test_vector_result.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/collision_bsp_test_flags.h"
#include "headers/blam_data_globals.h"


extern real_point3d *object_get_origin(int object_index, real_point3d *origin);
extern uint8_t collision_bsp_test_vector(unsigned int flags, const struct collision_bsp *bsp, int16_t breakable_surface_count, const uint8_t *breakable_surface_flags, const real_point3d *point, const real_vector3d *vector, double maximum_t, collision_bsp_test_vector_result *result);

int biped_find_ground_surface(int biped_index, float ground_distance,
                              const real_vector3d *ground_direction,
                              real_point3d *surface_point, real_vector3d *surface_normal)
{
    const collision_bsp *bsp = global_collision_bsp;

    real_point3d origin;
    object_get_origin(biped_index, &origin);

    real_vector3d test_vector;
    test_vector.n[0] = ground_direction->n[0] * ground_distance;
    test_vector.n[1] = ground_direction->n[1] * ground_distance;
    test_vector.n[2] = ground_direction->n[2] * ground_distance;

    /* start the cast slightly above the origin so a biped standing on the ground still finds it */
    origin.n[0] += global_up3d->n[0] * 0.40000001f;
    origin.n[1] += global_up3d->n[1] * 0.40000001f;
    origin.n[2] += global_up3d->n[2] * 0.40000001f;

    collision_bsp_test_vector_result result;
    if (!collision_bsp_test_vector((1u << _collision_bsp_test_front_facing_surfaces_bit), bsp, 0, nullptr, &origin, &test_vector, 3.4028235e38f, &result))
        return -1;

    if (surface_point)
    {
        surface_point->n[0] = test_vector.n[0] * result.t + origin.n[0];
        surface_point->n[1] = test_vector.n[1] * result.t + origin.n[1];
        surface_point->n[2] = test_vector.n[2] * result.t + origin.n[2];
    }
    if (surface_normal)
    {
        surface_normal->n[0] = result.plane->n.n[0];
        surface_normal->n[1] = result.plane->n.n[1];
        surface_normal->n[2] = result.plane->n.n[2];
    }
    return result.surface_index;
}
