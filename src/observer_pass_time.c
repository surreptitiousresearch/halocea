/* observer_pass_time @ 0x837104B8 — integrate one local player's observer (smoothed camera) forward by
 * the frame's elapsed time. Skipped when the pending command requests an instant cut (flags bit 0x20).
 *
 * It first computes the per-parameter displacement between the last command's target and the observer's
 * current resolved state: the 8 linear scalars (focus position/offset/distance/fov) as plain
 * differences, and the orientation as the angular difference between the two forward/up frames
 * (built into rotation matrices and reduced by vector_from_matrices4x3). It then runs the polynomial /
 * acceleration / velocity / position integrator passes, and finally decrements the command's five
 * interpolation timers toward zero by the elapsed time.
 *
 * The command target params live in last_command.parameters (___u1 @+0x04), the current resolved params
 * in obs->positions (___u6 @+0xB0), the displacement derivative in obs->displacements (@+0x260) and the
 * interpolation timers in last_command.parameter_timers (___u5 @+0x54); the computation mirrors the
 * verified sibling observer_update_displacements. */

#include <stdint.h>
#include "headers/observer_globals.h"
#include "headers/observer_command_flags.h"
#include "headers/observer_derivative.h"
#include "headers/real_matrix4x3.h"
#include "headers/real_vector3d.h"

extern void matrix4x3_rotation_from_vectors(real_matrix4x3 *matrix, const real_vector3d *forward, const real_vector3d *up);
extern real_vector3d *vector_from_matrices4x3(const real_matrix4x3 *a, const real_matrix4x3 *b, real_vector3d *rotation);
extern void observer_update_polynomial(int16_t local_player_index);
extern void observer_update_accelerations(int16_t local_player_index);
extern void observer_update_velocities(int16_t local_player_index);
extern void observer_update_positions(int16_t local_player_index);

void observer_pass_time(int16_t local_player_index)
{
    observer *obs = &observer_globals.local_players[local_player_index];

    if ( (obs->pending_command->flags & (1u << _observer_command_freeze_camera_bit)) != 0 )
        return;   /* instant cut: no integration */

    {
        /* recovered: (char*)&last_command+0x04 -> last_command.parameters (___u1) */
        const float *target_params  = obs->last_command.parameters;
        /* recovered: (char*)obs+0xB0 -> obs->positions (___u6) */
        const float *current_params = obs->positions;
        /* recovered: (char*)obs+0x260 -> obs->displacements.focus_position.n */
        float *displacement = obs->displacements.focus_position.n;
        int index;

        /* 8 linear scalars: simple target - current differences */
        for ( index = 0; index < 8; index++ )
            displacement[index] = target_params[index] - current_params[index];

        /* orientation: angular difference between the two forward/up frames.
         * recovered: the pointers step by single floats (not real_vector3d units), so the frame
         * begins at scalar [index]==[8], matching the decompiler and observer_update_displacements. */
        for ( ; index < 14; index += 6 )
        {
            real_matrix4x3 current_rotation;
            real_matrix4x3 target_rotation;
            matrix4x3_rotation_from_vectors(&current_rotation,
                                            (const real_vector3d *)&current_params[index],
                                            (const real_vector3d *)&current_params[index + 3]);
            matrix4x3_rotation_from_vectors(&target_rotation,
                                            (const real_vector3d *)&target_params[index],
                                            (const real_vector3d *)&target_params[index + 3]);
            vector_from_matrices4x3(&current_rotation, &target_rotation,
                                    (real_vector3d *)&displacement[index]);
        }
    }

    observer_update_polynomial(local_player_index);
    observer_update_accelerations(local_player_index);
    observer_update_velocities(local_player_index);
    observer_update_positions(local_player_index);

    /* decrement the command's five interpolation timers toward zero */
    {
        /* recovered: (char*)&last_command+0x44 walk -> last_command.parameter_timers[0..4] (___u5 @+0x54) */
        float *timer = obs->last_command.parameter_timers;
        int i;
        for ( i = 0; i < 5; i++ )
        {
            float remaining = (timer[i] - observer_globals.dtime);
            if ( remaining <= 0.0f )
                remaining = 0.0f;
            timer[i] = remaining;
        }
    }
}
