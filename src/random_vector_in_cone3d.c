/* random_vector_in_cone3d @0x836C6A58 — random unit vector within a cone about
 * axis (between inner and outer cone half-angles) using the global random seed. */

#include <stdint.h>
#include "headers/real_vector3d.h"

extern uint32_t *get_global_random_seed_address(void);
extern real_vector3d *seed_random_vector_in_cone3d(uint32_t *seed, const real_vector3d *axis, float inner_cone_angle, float outer_cone_angle, real_vector3d *result);

real_vector3d *random_vector_in_cone3d(
        const real_vector3d *axis,
        float inner_cone_angle,
        float outer_cone_angle,
        real_vector3d *result)
{
    /* DEVIATION: result (incoming r6) is forwarded as the callee's 5th arg; the
     * decompiler mislabels it as an uninitialised local because the two float args
     * reserve GPR slots r4/r5 (float-slot-skip ABI), leaving result in r6. */
    return seed_random_vector_in_cone3d(
        get_global_random_seed_address(), axis, inner_cone_angle, outer_cone_angle, result);
}
