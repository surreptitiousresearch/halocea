/* observer_update_velocities @ 0x8370F150 — recompute each camera scalar's velocity from the active
 * quintic polynomial. While a group is still blending (timer not elapsed) the velocity is the analytic
 * derivative of the position polynomial; once the timer elapses the velocity decays to the value that
 * would carry the residual displacement out over one dtime step, unless the group is flagged to snap
 * (parameter_flags bit 1, or whole command flagged instant via flag 0x8), in which case it is zeroed.
 *
 * Velocity array is strided by observer_parameter_derivative_real_counts (group sizes summing to 11). */

#include <stdint.h>
#include "headers/observer_globals.h"
#include "headers/observer_command_flags.h"
#include "headers/blam_data_globals.h"

extern void *memset(void *dst, int c, unsigned int n);

void observer_update_velocities(int16_t local_player_index)
{
    observer *obs = &observer_globals.local_players[local_player_index];
    double inv_dt = (float)(1.0 / observer_globals.dtime);
    float *disp = obs->displacements.focus_position.n;
    float *vel  = obs->velocities.focus_position.n;
    float *a = obs->a, *b = obs->b, *c = obs->c, *d = obs->d, *e = obs->e;
    float *group_timer = &obs->last_command.parameter_timers[0];
    uint8_t *group_flags = &obs->last_command.___u4.parameter_flags[0]; /* ___u4 is a named union member in observer_command (not flattened) */
    int group = 0;

    do
    {
        int count = observer_parameter_derivative_real_counts[group];
        double remaining = (*group_timer - observer_globals.dtime);
        if ( remaining <= 0.0 )
        {
            int flags = obs->last_command.flags;
            if ( (flags & (1u << _observer_command_valid_bit)) != 0 )
            {
                int i;
                /* *group_flags bit1 = per-parameter "pinned/snap" (no named DB enum for parameter_flags);
                 * command force_time_bit collapses the residual to an instant snap */
                if ( (*group_flags & 2) != 0 || (flags & (1u << _observer_command_force_time_bit)) != 0 )
                {
                    memset(vel, 0, 4 * count);
                }
                else
                {
                    for ( i = 0; i < count; i++ )
                        vel[i] = -(disp[i] * (float)inv_dt);
                }
            }
        }
        else
        {
            int i;
            for ( i = 0; i < count; i++ )
            {
                double t = remaining;
                vel[i] = (((d[i] * (float)t) * 2.0f)
                              + (((c[i] * (float)(t * t)) * 3.0f)
                                      + (((a[i] * (float)((float)((float)(t * t) * t) * t)) * 5.0f)
                                              + ((b[i] * (float)((float)(t * t) * t)) * 4.0f))))
                       + e[i];
            }
        }
        group_timer++;
        group_flags++;
        {
            int stride = observer_parameter_derivative_real_counts[group];
            disp += stride; vel += stride;
            a += stride; b += stride; c += stride; d += stride; e += stride;
        }
        group++;
    }
    while ( group < 5 );
}
