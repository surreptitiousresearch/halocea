/* observer_update_positions @ 0x8370F778 — advance the resolved camera scalars by one frame. For each
 * of the 5 parameter groups: if the group is still blending, evaluate its quintic position polynomial to
 * get this frame's displacement; if its timer just elapsed, either snap straight to the command target
 * (whole command still valid) or carry out the residual at constant velocity. Linear groups accumulate
 * the displacement directly; the orientation group routes through observer_apply_rotational_displacement
 * so forward/up rotate rather than translate. Finally the forward/up basis is re-orthonormalized.
 *
 * The `a2` second parameter the decompiler invents is an uninitialized FP scratch register, not a real
 * argument — reconstructed here as local scratch. Scalars stride by observer_parameter_real_counts
 * (group sizes summing to 14); coefficient arrays a..f stride likewise. */

#include <stdint.h>
#include "headers/observer_globals.h"
#include "headers/observer_command_flags.h"
#include "headers/real_vector3d.h"
#include "headers/observer_parameter.h"
#include "headers/blam_data_globals.h"

extern void observer_apply_rotational_displacement(const real_vector3d *rotational_displacement, real_vector3d *forward, real_vector3d *up);
extern int _isnan(double x);
extern double __fabs(double x);
extern float __fsqrts(float x);

void observer_update_positions(int16_t local_player_index)
{
    observer *obs = &observer_globals.local_players[local_player_index];
    float displacement[14];           /* per-frame scalar deltas (was decompiler stack v51) */
    float *delta = displacement;
    float *resolved = obs->positions;                       /* ___u6: current resolved scalars */
    float *velocity = obs->velocities.focus_position.n;
    float *target = obs->last_command.focus_position.n;     /* command end-state (___u1) */
    float *a = obs->a, *b = obs->b, *c = obs->c, *d = obs->d, *e = obs->e, *f = obs->f;
    float *group_timer = &obs->last_command.parameter_timers[0];
    int group;
    int i;

    for ( i = 0; i < NUMBER_OF_OBSERVER_REAL_VELOCITIES; i++ )
        displacement[i] = 0.0f;

    for ( group = 0; group < NUMBER_OF_OBSERVER_PARAMETERS; group++ )
    {
        double remaining = (*group_timer - observer_globals.dtime);
        int deriv_count = observer_parameter_derivative_real_counts[group];

        if ( remaining > 0.0 )
        {
            /* still blending: evaluate the quintic position polynomial */
            for ( i = 0; i < deriv_count; i++ )
            {
                double t = remaining;
                delta[i] = ((e[i] * (float)t)
                         + ((d[i] * (float)(t * t))
                                 + ((c[i] * (float)((float)(t * t) * t))
                                         + ((a[i] * (float)((float)((float)((float)(t * t) * t) * t) * t))
                                                 + (b[i] * (float)((float)((float)(t * t) * t) * t))))))
                         + f[i];
            }
        }
        else if ( (obs->last_command.flags & (1u << _observer_command_valid_bit)) != 0 )
        {
            /* timer elapsed but command still valid: snap resolved scalars to the command target */
            int real_count = observer_parameter_real_counts[group];
            for ( i = 0; i < real_count; i++ )
                resolved[i] = target[i];
            goto next_group;
        }
        else
        {
            /* timer elapsed, no command: carry the residual displacement out at constant velocity */
            for ( i = 0; i < deriv_count; i++ )
                delta[i] = -(velocity[i] * observer_globals.dtime);
        }

        if ( group >= _observer_orientation )
            observer_apply_rotational_displacement((const real_vector3d *)delta,
                                                   (real_vector3d *)resolved,
                                                   (real_vector3d *)resolved + 1);
        else
            for ( i = 0; i < deriv_count; i++ )
                resolved[i] += delta[i];

    next_group:
        {
            int real_stride = observer_parameter_real_counts[group];
            int deriv_stride = observer_parameter_derivative_real_counts[group];
            group_timer++;
            resolved += real_stride;
            target += real_stride;
            delta += deriv_stride;
            velocity += deriv_stride;
            a += deriv_stride; b += deriv_stride; c += deriv_stride;
            d += deriv_stride; e += deriv_stride; f += deriv_stride;
        }
    }

    /* Re-orthonormalize the forward/up basis (resolved scalars 8..13). The block is only rebuilt when it
     * has drifted from orthonormal (each test tolerance 0.001 / 1e-4). */
    {
        float *fwd = &obs->positions[8];          /* forward at [8..10], up at [11..13] */
        int g = _observer_orientation;
        do
        {
            float *up = fwd + 3;
            double fwd_len_err = (((fwd[2] * fwd[2]) + ((fwd[0] * fwd[0]) + (fwd[1] * fwd[1]))) - 1.0f);
            int orthonormal;

            orthonormal = !(_isnan(fwd_len_err) || __fabs(fwd_len_err) >= 0.001);
            if ( orthonormal )
            {
                double up_len_err = (((up[2] * up[2]) + ((up[0] * up[0]) + (up[1] * up[1]))) - 1.0f);
                orthonormal = !(_isnan(up_len_err) || __fabs(up_len_err) >= 0.001);
                if ( orthonormal )
                {
                    double dot = ((fwd[1] * up[1]) + ((fwd[2] * up[2]) + (fwd[0] * up[0])));
                    orthonormal = !(_isnan(dot) || __fabs(dot) >= 0.001);
                }
            }

            if ( !orthonormal )
            {
                /* Gram-Schmidt rebuild: up = forward x (up x forward), then normalize both vectors. */
                double fz = fwd[2];
                double fx = fwd[0];
                double cross_x = ((up[2] * fwd[0]) - (up[0] * fwd[2]));   /* DEVIATION: second term is up[0]*fwd[2] per fmsubs @0x8370FB44 (f9 = up[0]*fwd[2] from lfs @0x8370FB2C); prior fwd[0]*fwd[2] was a transcription error */
                double cross_y = ((up[1] * fwd[2]) - (up[2] * fwd[1]));
                double tmp_z = (fwd[2] * ((up[2] * fwd[0]) - (up[0] * fwd[2])));
                double tmp_x = (fwd[0] * ((up[0] * fwd[1]) - (up[1] * fwd[0])));
                double tmp_y = (fwd[1] * ((up[1] * fwd[2]) - (up[2] * fwd[1])));
                double len;

                up[0] = (fwd[1] * ((up[0] * fwd[1]) - (up[1] * fwd[0])))
                      - (fwd[2] * ((up[2] * fwd[0]) - (up[0] * fwd[2])));
                up[1] = ((float)cross_y * (float)fz) - (float)tmp_x;
                up[2] = ((float)fx * (float)cross_x) - (float)tmp_y;

                len = __fsqrts(((fwd[2] * fwd[2]) + ((fwd[0] * fwd[0]) + (fwd[1] * fwd[1]))));
                if ( __fabs(len) >= 0.000099999997 )
                {
                    float inv = 1.0f / (float)len;
                    fwd[0] = fwd[0] * inv;
                    fwd[1] = fwd[1] * inv;
                    fwd[2] = inv * fwd[2];
                }
                len = __fsqrts(((up[2] * up[2]) + ((up[0] * up[0]) + (up[1] * up[1]))));
                if ( __fabs(len) >= 0.000099999997 )
                {
                    float inv = 1.0f / (float)len;
                    up[0] = up[0] * inv;
                    up[1] = up[1] * inv;
                    up[2] = up[2] * inv;
                }
            }

            fwd += observer_parameter_real_counts[g];
            g++;
        }
        while ( g < NUMBER_OF_OBSERVER_PARAMETERS );
    }
}
