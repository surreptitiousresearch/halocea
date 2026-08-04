/* collision_bsp_test_pill @0x837E2C50 — set up the swept-pill traversal state and run it from the BSP root.
 * Seeds result->t with the (clamped non-negative) maximum sweep distance so bsp3d_test_pill_recursive only keeps
 * nearer contacts. Returns TRUE if the pill struck anything.
 *
 * Deviation: Hex-Rays split `result` into a phantom a7 plus the real a8; the DB call sites pass a single result
 * pointer. radius(f1)/maximum_t(f2) are the two float args. */

#include <stdint.h>
#include "headers/test_pill_data.h"

extern uint8_t bsp3d_test_pill_recursive(test_pill_data *data, int child_index);

uint8_t collision_bsp_test_pill(const collision_bsp *bsp, const real_point3d *point,
                                        const real_vector3d *vector, float radius, float maximum_t,
                                        collision_bsp_test_pill_result *result)
{
    test_pill_data data;
    data.bsp = bsp;
    data.point = point;
    data.radius = radius;
    data.vector = vector;
    result->leaf_count = 0;
    data.result = result;
    data.stack_depth = 0;
    result->t = (maximum_t >= 0.0) ? maximum_t : 0.0;   /* fsel clamp to >= 0 */
    return bsp3d_test_pill_recursive(&data, 0);
}
