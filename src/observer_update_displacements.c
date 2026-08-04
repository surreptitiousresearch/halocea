/* observer_update_displacements @0x83710048 — recompute a local player's camera displacement block (the
 * distance still to travel toward the last command): the first 8 scalars (focus position/offset, distance,
 * FOV) are plain differences command - current; the orientation group (forward/up vector pairs, scalars
 * 8..13) is converted to a single rotation-delta vector by building rotation matrices from both frames and
 * differencing them (vector_from_matrices4x3), filling the last 3 displacement scalars. */

#include <stdint.h>
#include "headers/observer_globals.h"
#include "headers/real_matrix4x3.h"

extern void matrix4x3_rotation_from_vectors(real_matrix4x3 *matrix, const real_vector3d *forward, const real_vector3d *up);
extern real_vector3d *vector_from_matrices4x3(const real_matrix4x3 *a, const real_matrix4x3 *b, real_vector3d *rotation);

void observer_update_displacements(int16_t local_player_index)
{
    observer *obs = &observer_globals.local_players[local_player_index];
    const float *command_parameters = obs->last_command.parameters;
    const float *current_positions = obs->positions;
    float *displacements = obs->displacements.focus_position.n;

    __int16 scalar_index = 0;
    do
    {
        displacements[scalar_index] = command_parameters[scalar_index] - current_positions[scalar_index];
        ++scalar_index;
    }
    while ( scalar_index < 8 );

    while ( scalar_index < 14 )
    {
        real_matrix4x3 current_rotation;
        real_matrix4x3 command_rotation;
        matrix4x3_rotation_from_vectors(&current_rotation,
                                        (const real_vector3d *)&current_positions[scalar_index],
                                        (const real_vector3d *)&current_positions[scalar_index + 3]);
        matrix4x3_rotation_from_vectors(&command_rotation,
                                        (const real_vector3d *)&command_parameters[scalar_index],
                                        (const real_vector3d *)&command_parameters[scalar_index + 3]);
        vector_from_matrices4x3(&current_rotation, &command_rotation,
                                (real_vector3d *)&displacements[scalar_index]);
        scalar_index += 6;
    }
}
