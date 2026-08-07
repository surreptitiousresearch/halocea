/* observer_update_accelerations @ 0x8370EF98 — recompute each camera scalar's acceleration as the
 * second derivative of the active quintic polynomial. If any scalar's acceleration exceeds the group's
 * configured maximum, the whole blend for that group (and every other group sharing the same timer) is
 * snapped to completion by zeroing those timers — preventing visible camera jerk. When a group's timer
 * has already elapsed its accelerations are simply cleared.
 *
 * Acceleration array is strided by observer_parameter_derivative_real_counts (sizes summing to 11). */

#include <stdint.h>
#include <string.h>
#include "headers/observer_globals.h"
#include "headers/blam_data_globals.h"


void observer_update_accelerations(int16_t local_player_index)
{
    observer *obs = &observer_globals.local_players[local_player_index];
    float *accel = obs->accelerations.focus_position.n;
    float *a = obs->a, *b = obs->b, *c = obs->c, *d = obs->d;
    float *group_timer = &obs->last_command.parameter_timers[0];
    int group = 0;

    do
    {
        double remaining = (*group_timer - observer_globals.dtime);
        int count = observer_parameter_derivative_real_counts[group];
        if ( remaining <= 0.0 )
        {
            memset(accel, 0, 4 * count);
        }
        else
        {
            int i;
            for ( i = 0; i < count; i++ )
            {
                double t = remaining;
                double value = ((d[i] * 2.0f)
                             + (((c[i] * (float)t) * 6.0f)
                                     + (((a[i] * (float)((float)(t * t) * t)) * 20.0f)
                                             + ((b[i] * (float)(t * t)) * 12.0f))));
                accel[i] = (float)value;

                if ( value > observer_maximum_accelerations[group] || value < -observer_maximum_accelerations[group] )
                {
                    int g;
                    for ( g = 0; g < 5; g++ )
                    {
                        if ( g != group
                          && obs->last_command.parameter_timers[g] == *group_timer )
                            obs->last_command.parameter_timers[g] = 0.0f;
                    }
                    *group_timer = 0.0f;
                }
            }
        }
        group_timer++;
        {
            int stride = observer_parameter_derivative_real_counts[group];
            accel += stride;
            a += stride; b += stride; c += stride; d += stride;
        }
        group++;
    }
    while ( group < 5 );
}
