/* actor_move_transform_avoidance_vector @0x837C83C0 — transform a local-space avoidance vector into world
 * space through the avoidance frame's basis: direction = forward*x + left*y + up*z, accumulated in place
 * over a zero-initialized output exactly as compiled. */

#include "headers/vector_avoidance_data.h"
#include "headers/blam_data_globals.h"


void actor_move_transform_avoidance_vector(vector_avoidance_data *avoidance_data,
        const real_vector3d *avoidance_vector, real_vector3d *direction_vector)
{
    *direction_vector = *global_zero_vector3d;

    direction_vector->n[0] += avoidance_data->forward.n[0] * avoidance_vector->n[0];
    direction_vector->n[1] += avoidance_data->forward.n[1] * avoidance_vector->n[0];
    direction_vector->n[2] += avoidance_data->forward.n[2] * avoidance_vector->n[0];

    direction_vector->n[0] += avoidance_data->left.n[0] * avoidance_vector->n[1];
    direction_vector->n[1] += avoidance_data->left.n[1] * avoidance_vector->n[1];
    direction_vector->n[2] += avoidance_data->left.n[2] * avoidance_vector->n[1];

    direction_vector->n[0] += avoidance_data->up.n[0] * avoidance_vector->n[2];
    direction_vector->n[1] += avoidance_data->up.n[1] * avoidance_vector->n[2];
    direction_vector->n[2] += avoidance_data->up.n[2] * avoidance_vector->n[2];
}
