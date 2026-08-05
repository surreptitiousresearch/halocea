/* path_3d_available @0x837D2298 — tests whether a straight-line 3D path from `start_point` to the
 * point encoded in `destination_reference` is clear of collision.
 *
 * `destination_reference` is read-only despite its `collision_bsp_test_vector_result*` shape: its raw
 * `t`/`plane`/`surface_index` words are reinterpreted as the x/y/z of the destination point (they are
 * exactly 3 consecutive 4-byte fields, the same size/shape as a `real_point3d`) — an established
 * type-pun in this pathfinding subsystem. disasm_range(0x837D2298, 0x837D23BC) confirms
 * `collision_bsp_test_vector`'s own `result` output is a SEPARATE local scratch buffer this function
 * owns, NOT `destination_reference` — an earlier pass through this function wrongly assumed
 * `destination_reference` gets clobbered by the call; it never is.
 *
 * DEVIATION: the decompiler renders `avoidance_distance` as a `double` and shows it read nowhere in
 * the body; disasm confirms it is a plain `float` that occupies the usual dead-GPR-shadow slot (r5)
 * and is never actually used by this function — kept as a real parameter since every caller still
 * computes and passes a value for it (matching the established `path_3d_build_path` signature).
 *
 * `path_available_out` and `hit_result_out` are both optional (NULL-checked). `path_available_out`
 * gets the same 0/1 as the return value. `hit_result_out[0..2]` receives a raw copy of
 * `destination_reference`'s own (unmodified) t/plane/surface_index words. Verified against 3
 * established callers where both are frequently passed NULL. */

#include <stdint.h>
#include <float.h>

#include "headers/structure_bsp.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/collision_bsp_test_vector_result.h"

extern uint8_t collision_bsp_test_vector(unsigned int flags, const struct collision_bsp *bsp, int16_t breakable_surface_count, const uint8_t *breakable_surface_flags, const real_point3d *point, const real_vector3d *vector, float maximum_t, collision_bsp_test_vector_result *result);

uint8_t path_3d_available(structure_bsp *structure_bsp, const real_point3d *start_point,
        float avoidance_distance, const collision_bsp_test_vector_result *destination_reference,
        uint8_t *path_available_out, float *hit_result_out)
{
    (void)avoidance_distance; /* confirmed unused — see DEVIATION above */

    real_vector3d ray_vector;
    ray_vector.n[0] = *(const float *)&destination_reference->t - start_point->n[0];
    ray_vector.n[1] = *(const float *)&destination_reference->plane - start_point->n[1];
    ray_vector.n[2] = *(const float *)&destination_reference->surface_index - start_point->n[2];

    collision_bsp_test_vector_result hit_result;
    uint8_t obstructed = 0;
    if (collision_bsp_test_vector(1u, (const struct collision_bsp *)structure_bsp->collision_bsp.address,
            0, 0, start_point, &ray_vector, FLT_MAX, &hit_result)
        && hit_result.t < 1.0f)
    {
        float remaining_t = 1.0f - hit_result.t;
        obstructed = (uint8_t)((ray_vector.n[1] * ray_vector.n[1]
                        + (ray_vector.n[0] * ray_vector.n[0] + ray_vector.n[2] * ray_vector.n[2]))
                    * (remaining_t * remaining_t) >= 0.1f);
    }

    uint8_t available = obstructed ? 0 : 1;

    if (path_available_out)
        *path_available_out = available;

    if (hit_result_out)
    {
        /* raw dword bit-copies into the caller's float[3] — no numeric conversion */
        ((uint32_t *)hit_result_out)[0] = *(const uint32_t *)&destination_reference->t;
        ((uint32_t *)hit_result_out)[1] = *(const uint32_t *)&destination_reference->plane;
        ((uint32_t *)hit_result_out)[2] = *(const uint32_t *)&destination_reference->surface_index;
    }

    return available;
}
