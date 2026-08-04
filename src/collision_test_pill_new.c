/* collision_test_pill_new @0x83773D20 — sweep-test a pill (capsule) of `radius` along `vector` from
 * `point` against the global structure collision BSP (no breakable-surface filtering), filling `collision`
 * with a "structure BSP surface hit" (type 2) result on success or leaving it at the "no hit" defaults
 * (type -1, t=1.0) otherwise, then always resolving `collision->point` to the endpoint reached (fraction
 * `collision->t` along `vector`) and zeroing `collision->plane.normal`.
 *
 * DEVIATION: the decompiler renders the 6th (and only pointer/struct) parameter as two separate params —
 * a `collision_result *collision` never referenced in the body, and a bogus `int a7` used throughout; the
 * DB prototype confirms there is only one, `collision` — reproduced using it directly for every field
 * write below. The decompiler also shows `collision->plane.normal` being set from the BSP test's `normal`
 * out-param on a successful hit, but that write is unconditionally overwritten by zero a few lines later
 * (nothing reads it in between) — omitted as provably dead, matching this session's convention for such
 * cases. `flags`/`ignore_object_index` are unused in this simplified pill-test variant. */

#include <stdint.h>
#include "headers/collision_bsp.h"
#include "headers/collision_result.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/collision_result_type.h"
#include "headers/blam_data_globals.h"


#include "headers/collision_bsp.h"
extern uint8_t collision_bsp_test_pill_new(const collision_bsp *bsp, int16_t breakable_surface_count, const uint8_t *breakable_surface_flags, const real_point3d *point, const real_vector3d *vector, float radius, float *t, real_vector3d *normal);

uint8_t collision_test_pill_new(unsigned int flags, const real_point3d *point, const real_vector3d *vector, float radius, int ignore_object_index, collision_result *collision)
{
    collision->type = -1;
    collision->start_location.leaf_index = -1;
    collision->start_location.cluster_index = -1;
    collision->location.leaf_index = -1;
    collision->location.cluster_index = -1;
    collision->t = 1.0f;

    float t;
    real_vector3d normal;
    unsigned __int8 hit = collision_bsp_test_pill_new(global_collision_bsp, 0, nullptr, point, vector, radius,
        &t, &normal);

    if ( hit )
    {
        collision->t = t;
        collision->material_type = -1;
        collision->type = collision_result_structure;
        collision->plane.d = 3.4028235e38f;
        collision->surface_index = -1;
        collision->plane_designator = -1;
        collision->flags = 0;
        collision->breakable_surface_index = 0;
        collision->material_index = -1;
    }

    collision->point.n[0] = vector->n[0] * collision->t + point->n[0];
    collision->point.n[1] = vector->n[1] * collision->t + point->n[1];
    collision->point.n[2] = vector->n[2] * collision->t + point->n[2];
    collision->plane.normal.n[0] = 0.0f;
    collision->plane.normal.n[1] = 0.0f;
    collision->plane.normal.n[2] = 0.0f;

    return hit;
}
