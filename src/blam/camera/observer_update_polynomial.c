/* observer_update_polynomial @ 0x8370ED48 — rebuild the per-scalar quintic interpolation polynomials
 * for one local player's camera. For each of the 5 parameter groups that is still actively blending
 * (last_command valid and its timer not yet elapsed), solve the boundary-value problem from the current
 * displacement/velocity/acceleration to a zero end-state over the remaining timer, producing the a..e
 * coefficients (f holds the constant term = current displacement). The position group (0) additionally
 * folds in the command's focus_velocity (scaled by 30 ticks/sec) as a non-zero end velocity.
 *
 * Coefficient arrays a..e are float[11] strided by observer_parameter_derivative_real_counts
 * (group sizes summing to 11); the derivative state arrays are observer_derivatives strided likewise. */

#include <stdint.h>
#include "headers/observer_globals.h"
#include "headers/observer_command_flags.h"
#include "headers/observer_parameter.h"
#include "headers/blam_data_globals.h"


void observer_update_polynomial(int16_t local_player_index)
{
    observer *obs = &observer_globals.local_players[local_player_index];
    float *accel = obs->accelerations.focus_position.n;
    float *disp  = obs->displacements.focus_position.n;
    float *vel   = obs->velocities.focus_position.n;
    float *a = obs->a, *b = obs->b, *c = obs->c, *d = obs->d, *e = obs->e, *f = obs->f;
    float *group_timer = &obs->last_command.parameter_timers[0];
    int group = 0;

    do
    {
        if ( (obs->last_command.flags & (1u << _observer_command_valid_bit)) != 0 && *group_timer > (double)observer_globals.dtime )
        {
            float inv_t  = 1.0f / *group_timer;
            float inv_t2 = inv_t * inv_t;
            float inv_t3 = inv_t2 * inv_t;
            int count = observer_parameter_derivative_real_counts[group];
            int i;
            for ( i = 0; i < count; i++ )
            {
                a[i] = ((accel[i] * inv_t3) * 0.5f)
                     - (((disp[i] * (inv_t3 * inv_t)) * 6.0f)
                             + ((inv_t3 * vel[i]) * 3.0f));
                b[i] = -((accel[i] * inv_t2)
                              - (((disp[i] * (inv_t3 * inv_t)) * 15.0f)
                                      + ((inv_t3 * vel[i]) * 7.0f)));
                c[i] = ((accel[i] * inv_t) * 0.5f)
                     - (((disp[i] * inv_t3) * 10.0f)
                             + ((inv_t2 * vel[i]) * 4.0f));
                d[i] = 0.0f;
                e[i] = 0.0f;
                f[i] = disp[i];

                if ( group == _observer_focus_position )
                {
                    /* position group: blend toward the command's focus velocity (30 ticks/sec) */
                    float v = obs->last_command.focus_velocity.n[i] * 30.0f;
                    a[i] = -(((v * (inv_t3 * inv_t)) * 3.0f) - a[i]);
                    b[i] = ((v * inv_t3) * 8.0f) + b[i];
                    c[i] = -(((v * inv_t2) * 6.0f) - c[i]);
                    e[i] = e[i] + v;
                }
            }
        }
        group_timer++;
        {
            int stride = observer_parameter_derivative_real_counts[group];
            accel += stride; disp += stride; vel += stride;
            a += stride; b += stride; c += stride; d += stride; e += stride; f += stride;
        }
        group++;
    }
    while ( group < NUMBER_OF_OBSERVER_PARAMETERS );
}
