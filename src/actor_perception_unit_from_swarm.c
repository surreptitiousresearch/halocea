/* actor_perception_unit_from_swarm @0x837D59F0 — given a sensing position and a swarm-controlling actor,
 * pick which unit of that actor's swarm (or, for a non-swarm actor, which unit in its controlled-unit chain)
 * best represents the perceived target: the closest one, biasing toward the currently-tracked unit (x0.36)
 * and away from components flagged unsuitable (x2.25). Optionally stamps every candidate object's
 * magic_number with the global object marker.
 *
 * 2026-07-13 SIGNATURE CORRECTED (binary-verified at both call sites): the database prototype's parameter
 * names were shifted one slot. True roles, now used directly:
 *   r3 sense_position       — actor_position_data*; only body_position (floats [3..5]) is read
 *   r4 actor_index          — the swarm-controlling actor
 *   r5 preferred_unit_index — currently-tracked unit, distance-biased x0.36
 *   r6 mark_units           — stamp candidates with the global object marker
 *   r7 unused               — Hex-Rays phantom (GPR slot reserved by no real arg)
 * Body fully typed (swarm_datum / swarm_component_datum / unit_datum). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/swarm_component_flags.h"
#include "headers/object_header_datum.h"
#include "headers/actor_datum.h"
#include "headers/unit_datum.h"
#include "headers/swarm_datum.h"
#include "headers/swarm_component_datum.h"
#include "headers/actor_position_data.h"
#include "headers/real_point3d.h"
#include "headers/blam_data_globals.h"

extern real_point3d *object_get_origin(int object_index, real_point3d *origin);

int actor_perception_unit_from_swarm(actor_position_data *sense_position, int actor_index,
                                     int preferred_unit_index, uint8_t mark_units)
{
    int best_unit = -1;
    actor_datum *actor = DATUM_GET(actor_data, actor_datum, actor_index);
    const real_point3d *sense = &sense_position->body_position;

    if ( actor->meta.swarm_cache_index == -1 )  /* not a swarm: walk the object's controlled-unit chain */
    {
        int unit = actor->meta.swarm_unit_index;
        float best_distance = 3.4028235e38f;
        if ( unit != -1 )
        {
            unsigned __int8 mark = mark_units;
            do
            {
                unit_datum *unit_object = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit)->datum);
                real_point3d origin;
                object_get_origin(unit, &origin);
                float dx = sense->n[0] - origin.n[0];
                float dy = sense->n[1] - origin.n[1];
                float dz = sense->n[2] - origin.n[2];
                float distance = (dy * dy) + ((dz * dz) + (dx * dx));
                if ( unit == preferred_unit_index )
                    distance = distance * 0.36000001f;
                if ( distance < best_distance )
                {
                    best_distance = distance;
                    best_unit = unit;
                }
                if ( mark )
                {
                    if ( unit_object->object.magic_number != global_object_marker )
                        unit_object->object.magic_number = global_object_marker;
                }
                unit = unit_object->unit.swarm_next_unit_index;
            }
            while ( unit != -1 );
        }
        return best_unit;
    }

    float best_distance = 3.4028235e38f;
    swarm_datum *swarm = DATUM_GET(swarm_data, swarm_datum, actor->meta.swarm_cache_index);
    if ( swarm->unit_count <= 0 )
        return best_unit;

    unsigned __int8 mark = mark_units;
    int marker = global_object_marker;
    for ( int i = 0; i < swarm->unit_count; i = (int16_t)(i + 1) )
    {
        swarm_component_datum *component = DATUM_GET(swarm_component_data, swarm_component_datum,
                                                              swarm->component_indices[i]);
        float dx = sense->n[0] - component->position.x;
        float dz = sense->n[2] - component->position.z;
        float dy = sense->n[1] - component->position.y;
        float distance = (dy * dy) + ((dz * dz) + (dx * dx));
        if ( (component->flags & (1u << _swarm_component_attached_bit)) != 0 )
            distance = distance * 2.25f;
        else if ( swarm->unit_indices[i] == preferred_unit_index )
            distance = distance * 0.36000001f;
        if ( distance < best_distance )
        {
            best_distance = distance;
            best_unit = swarm->unit_indices[i];
        }
        if ( mark )
        {
            unit_datum *unit_object = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, swarm->unit_indices[i])->datum);
            if ( unit_object->object.magic_number != marker )
            {
                unit_object->object.magic_number = marker;
                marker = global_object_marker;
            }
        }
    }
    return best_unit;
}
