/* wind_variance_initialize @0x837C1CA8 — precompute the cyclic wind-variance noise table. First it seeds 8
 * random direction keyframes per axis (stride 8 in the 64-entry table); then it fills the 7 intermediate
 * entries between each pair of keyframes with a uniform cubic spline through the four surrounding keyframes,
 * wrapping cyclically.
 *
 * Deviation: the decompiler's keyframe/substep counters are stack-overlapped (note the OVERLAPPED __int64s);
 * reconstructed as explicit keyframe/substep/axis loops. t0 = keyframe-1, h = 1, t = keyframe + substep/8. */

#include "headers/wind_globals.h"
#include "headers/real_vector3d.h"
#include <stdint.h>

#include "headers/real_vector3d.h"
extern uint32_t *get_global_random_seed_address(void);
extern real_vector3d *seed_random_direction3d(uint32_t *seed, real_vector3d *direction);
extern void uniform_cubic_spline_vector3d(real_vector3d *result, const real_vector3d *f0, const real_vector3d *f1, const real_vector3d *f2, const real_vector3d *f3, float t0, float h, float t);

void wind_variance_initialize(void)
{
    for ( int16_t keyframe = 0; keyframe < 8; keyframe = (int16_t)(keyframe + 1) )
        for ( int16_t axis = 0; axis < 3; axis = (int16_t)(axis + 1) )
            seed_random_direction3d(get_global_random_seed_address(),
                                    &wind_globals.variance[axis][8 * keyframe]);

    for ( int keyframe = 0; keyframe < 8; keyframe = (int16_t)(keyframe + 1) )
    {
        int kf = (uint16_t)keyframe;
        for ( int16_t substep = 1; substep < 8; substep = (int16_t)(substep + 1) )
        {
            for ( int axis = 0; axis < 3; axis = (int16_t)(axis + 1) )
            {
                uniform_cubic_spline_vector3d(
                    &wind_globals.variance[axis][8 * keyframe + substep],
                    &wind_globals.variance[axis][8 * ((kf - 1) & 7)],
                    &wind_globals.variance[axis][8 * kf],
                    &wind_globals.variance[axis][8 * ((kf + 1) & 7)],
                    &wind_globals.variance[axis][8 * ((kf + 2) & 7)],
                    (float)(kf - 1),
                    1.0f,
                    (((float)substep * (float)0.125) + (float)keyframe));
            }
        }
    }
}
