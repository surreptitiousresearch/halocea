/* collision_bsp_test_pill_new @0x837E17B8 — set up a test_pill_new_data recursion frame and kick off the
 * BSP-tree pill sweep from the root child (0, parametric range [0,1]).
 *
 * DEVIATION: the decompiler fabricated a bogus 9th `real_vector3d *a9` parameter and mixed up which pointer
 * feeds `*t = FLT_MAX` vs `data.normal`; the DB's own prototype (8 params) and disasm_range(0x837E17B8,
 * 0x837E1828) agree: `radius` (float) consumes an FPR but still reserves its positional GPR slot (this
 * compiler's ABI convention), so `t` arrives in r9 and `normal` in r10 — one slot later than the decompiler
 * assumed. The FLT_MAX sentinel write is `*t = FLT_MAX`, not `normal->n[0]`. */

#include <stdint.h>
#include "headers/test_pill_new_data.h"
#include "headers/contents.h"

extern uint8_t collision_bsp_test_pill_new_recursive(test_pill_new_data *data, int child_index, float t0, float t1);

uint8_t collision_bsp_test_pill_new(const collision_bsp *bsp, int16_t breakable_surface_count, const uint8_t *breakable_surface_flags, const real_point3d *point, const real_vector3d *vector, float radius, float *t, real_vector3d *normal)
{
    test_pill_new_data data;

    data.bsp = bsp;
    data.breakable_surface_count = breakable_surface_count;
    data.breakable_surface_flags = breakable_surface_flags;
    data.point = point;
    *t = 3.4028235e38f;
    data.vector = vector;
    data.t = t;
    data.normal = normal;
    data.last_leaf_index = -1;
    data.last_contents = _contents_unknown;
    data.last_plane_designator = -1;
    data.radius = radius;

    return collision_bsp_test_pill_new_recursive(&data, 0, 0.0f, 1.0f);
}
