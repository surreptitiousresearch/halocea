/* actor_path_3d_available @0x837C8880 — returns whether 3D (physics-driven) pathing should be USED for
 * an actor's move toward `destination_point`, as opposed to a simpler/direct movement. The special case
 * only applies when the actor is riding a vehicle-driver seat of type 4, that vehicle's tag has a
 * positive avoidance distance (resolved to vehicle_definition.vehicle.ai_avoidance_distance @+904),
 * and the actor's facing is confident enough (actor->control.vector_avoidance_rotation_emergency_instantaneous
 * @+1516 > 0.9). In that case, 3D pathing is
 * reported UNAVAILABLE (0) once the actor is already facing close enough to the destination bearing
 * (dot product with `facing_vector` > 0.984, i.e. within ~10 degrees) — no avoidance maneuvering needed,
 * a direct move suffices; otherwise it stays available (1) so the caller routes around obstacles while
 * turning. Outside the vehicle-seat/avoidance-tag gate, always reports available (1).
 *
 * Clean decompile, no corruption — matches disasm_range(0x837C8880, 0x837C89B4) directly; only the
 * tag-lookup idiom (object index -> object header -> TAG_INSTANCE) was expanded from the established
 * pattern already used in `actor_combat_update.c`. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/actor_datum.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/vehicle_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/actor_vehicle_driver_type.h"
#include "headers/blam_data_globals.h"


extern float normalize3d(real_vector3d *v);

uint8_t actor_path_3d_available(int actor_index, const real_point3d *destination_point,
        float *avoidance_distance_reference)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    float avoidance_distance = 0.0f;
    uint8_t available = 1;

    if (actor->input.vehicle_driver_type == _actor_vehicle_driver_directional_flying)
    {
        object_datum *vehicle_object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, actor->input.vehicle_index)->datum;
        vehicle_definition *vehicle_tag = TAG_GET(vehicle_definition, *(int *)vehicle_object);
        avoidance_distance = vehicle_tag->vehicle.ai_avoidance_distance; /* abs +904 = vehicle.ai_avoidance_distance */

        if (avoidance_distance > 0.0f && actor->control.vector_avoidance_rotation_emergency_instantaneous > 0.9f)
        {
            real_vector3d bearing;
            bearing.n[0] = destination_point->n[0] - actor->input.position.body_position.n[0];
            bearing.n[1] = destination_point->n[1] - actor->input.position.body_position.n[1];
            bearing.n[2] = destination_point->n[2] - actor->input.position.body_position.n[2];

            if (normalize3d(&bearing) > 0.0)
            {
                float facing_dot = actor->input.facing_vector.n[0] * bearing.n[0]
                        + actor->input.facing_vector.n[1] * bearing.n[1]
                        + actor->input.facing_vector.n[2] * bearing.n[2];
                available = facing_dot <= 0.984f;
            }
        }
    }

    if (avoidance_distance_reference)
        *avoidance_distance_reference = avoidance_distance;

    return available;
}
