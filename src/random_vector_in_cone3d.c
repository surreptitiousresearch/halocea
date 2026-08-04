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
    /* result (incoming r6) forwarded as the 5th arg; the decompiler mislabels
     * it as an uninitialised local because PPC reserves r5/r6 for the two float
     * args, leaving result in r7. */
    return seed_random_vector_in_cone3d(
        get_global_random_seed_address(), axis, inner_cone_angle, outer_cone_angle, result);
}
