/* actor_perception_find_sense_position @0x837D58F8 — pick the sensing position an actor should use to perceive
 * a target at `position`. For a swarm actor, find the swarm unit whose component position is closest to the
 * target and sample that unit's position; otherwise just copy the actor's own cached sense position. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/swarm_datum.h"
#include "headers/swarm_component_datum.h"
#include "headers/real_point3d.h"
#include "headers/actor_position_data.h"
#include <string.h>
#include "headers/blam_data_globals.h"

extern void actor_input_sample_position(int actor_index, int unit_index, actor_position_data *position);

void actor_perception_find_sense_position(int actor_index, const real_point3d *position,
                                          int prop_index, actor_position_data *sense_position)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    if ( actor->meta.swarm )
    {
        int closest_unit = -1;
        float closest_distance_squared = 3.4028235e38f;
        swarm_datum *swarm = DATUM_GET(swarm_data, swarm_datum, actor->meta.swarm_cache_index);
        int unit_count = swarm->unit_count;
        for ( int i = 0; i < unit_count; i = (int16_t)(i + 1) )
        {
            swarm_component_datum *component = DATUM_GET(swarm_component_data, swarm_component_datum,
                                                                  swarm->component_indices[i]);
            float dz = position->n[2] - component->position.z;
            float dx = position->n[0] - component->position.x;
            float dy = position->n[1] - component->position.y;
            float distance_squared = (dy * dy) + ((dx * dx) + (dz * dz));
            if ( distance_squared < closest_distance_squared )
            {
                closest_distance_squared = distance_squared;
                closest_unit = swarm->unit_indices[i];  /* raw &swarm[4*i+24] -> unit_indices[i] (offset 24) */
            }
        }
        actor_input_sample_position(actor_index, closest_unit, sense_position);
    }
    else
    {
        memcpy(sense_position, &actor->input.position, sizeof(actor_position_data));
    }
}
