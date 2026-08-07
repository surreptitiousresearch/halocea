/* path_3d_available @0x837D2298 — tests whether a straight-line 3D path from `start_point` to
 * `end_point` is clear of collision.
 *
 * DEVIATION: an earlier pass typed `end_point` as a `collision_bsp_test_vector_result*` and punned its
 * t/plane/surface_index words into x/y/z. That was wrong on both counts: the DB prototype for
 * 0x837D2298 is `(structure_bsp *, const real_point3d *start_point, float, const real_point3d
 * *end_point, unsigned __int8 *finishing_path_reference, real_point3d *path_endpoint)`, and the
 * disassembly reads r6 with `lfs 0/4/8(r6)` @0x837D22BC-0x837D22CC — float loads, not word puns.
 * `collision_bsp_test_vector`'s own `result` output is a SEPARATE local scratch buffer this function
 * owns, and `end_point` is never clobbered by the call.
 *
 * DEVIATION: the decompiler renders `avoidance_distance` as a `double` and shows it read nowhere in
 * the body; disasm confirms it is a plain `float` that occupies the usual dead-GPR-shadow slot (r5)
 * and is never actually used by this function — kept as a real parameter since every caller still
 * computes and passes a value for it (matching the established `path_3d_build_path` signature).
 *
 * `path_available_out` and `path_endpoint` are both optional (NULL-checked). `path_available_out`
 * gets the same 0/1 as the return value. `path_endpoint` receives an unmodified copy of `end_point`
 * (3 lwz/stw @0x837D22F8-0x837D2308 / 0x837D23A8-0x837D23B0 — the usual lowering of a 12-byte struct
 * copy). Verified against 3 established callers where both are frequently passed NULL. */

#include <stdint.h>
#include <float.h>

#include "headers/structure_bsp.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/collision_bsp_test_vector_result.h"
#include "headers/collision_bsp.h"
extern uint8_t collision_bsp_test_vector(unsigned int flags, const collision_bsp *bsp, int16_t breakable_surface_count, const uint8_t *breakable_surface_flags, const real_point3d *point, const real_vector3d *vector, float maximum_t, collision_bsp_test_vector_result *result);

uint8_t path_3d_available(structure_bsp *structure_bsp, const real_point3d *start_point,
        float avoidance_distance, const real_point3d *end_point,
        uint8_t *path_available_out, real_point3d *path_endpoint)
{
    (void)avoidance_distance; /* confirmed unused — see DEVIATION above */

    real_vector3d ray_vector;
    ray_vector.n[0] = end_point->n[0] - start_point->n[0];
    ray_vector.n[1] = end_point->n[1] - start_point->n[1];
    ray_vector.n[2] = end_point->n[2] - start_point->n[2];

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

    if (path_endpoint)
        *path_endpoint = *end_point;

    return available;
}
