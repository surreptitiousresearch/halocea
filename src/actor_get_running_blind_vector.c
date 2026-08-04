/* actor_get_running_blind_vector @0x8371E290 — fetch the direction a panicking actor should flee along,
 * normalised. Returns 1 if a vector was produced, 0 otherwise. Two sources, in priority order: an explicit
 * stored flee vector (control.moving_towards_vector, valid when control.moving set), or the vector from
 * the actor toward a remembered path destination point (control.path.destination.point minus
 * input.position.body_position, valid when control.path.path.valid set). A degenerate (zero-length) result
 * reports failure. Suppressed entirely when the actor's swarm flag (meta.swarm) is set. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"

extern float normalize3d(real_vector3d *v);

uint8_t actor_get_running_blind_vector(uint16_t actor_index, real_vector3d *run_vector)
{
    int have_vector = 0;
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    if ( !actor->meta.swarm )
    {
        if ( actor->control.moving )
        {
            *run_vector = actor->control.moving_towards_vector;
            have_vector = 1;
        }
        else if ( actor->control.path.path.valid )
        {
            run_vector->n[0] = actor->control.path.destination.point.x - actor->input.position.body_position.x;
            run_vector->n[1] = actor->control.path.destination.point.y - actor->input.position.body_position.y;
            run_vector->n[2] = actor->control.path.destination.point.z - actor->input.position.body_position.z;
            have_vector = 1;
        }
    }

    if ( !(unsigned __int8)have_vector )
        return have_vector;
    if ( normalize3d(run_vector) != 0.0 )
        return have_vector;
    return 0;
}