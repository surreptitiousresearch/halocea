/* action_vehicle_desirable @0x83821538 — is `vehicle_index` a reasonable vehicle for `actor_index` to try
 * entering right now? Refuses outright if the vehicle's damage_flags bit2 is set (e.g. destroyed). A scripted
 * request always succeeds. Otherwise: pick the distance threshold (continue-distance if the actor is already
 * attempting entry, else start-distance), and require either `already_inside` or the actor to be within that
 * distance of the vehicle's origin; when accepted and not already inside/attempting, also require the
 * vehicle's velocity to be below a small threshold (~0.0167 units/tick, i.e. roughly stationary) unless it's
 * a scripted/forced acceptance. Finally (unless `already_inside`), the vehicle must be upright enough
 * (`object.up.z >= 0.5`) or entry is refused regardless.
 *
 * DEVIATION — dead-float-shadow-GPR pattern: `attempt_start_distance`/`attempt_continue_distance` (params 4-5,
 * both float) each consume a dead GPR shadow, so the DB's own decompile fabricates two extra trailing
 * parameters (`a8`, `a9`) that hold the TRUE `already_inside`/`already_attempting_entry` values (confirmed via
 * disasm_range 0x83821538-0x8382158C: only ONE `mr` moves r8 into a saved register — matching a8's usage — and
 * r9 is used live later matching a9's usage), while its own declared `already_inside`/`already_attempting_entry`
 * parameters (6th/7th) are dead. The real signature is the 7-param one already used at this function's only
 * call site (action_vehicle_setup_specific.c), confirmed there via disasm too. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/real_point3d.h"
#include "headers/object_damage_flags.h"
#include "headers/blam_data_globals.h"

extern real_point3d *object_get_origin(int object_index, real_point3d *origin);

uint8_t action_vehicle_desirable(int actor_index, int vehicle_index, uint8_t scripted_request, float attempt_start_distance, float attempt_continue_distance, uint8_t already_inside, uint8_t already_attempting_entry)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    object_datum *vehicle = (DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, vehicle_index)->datum);

    if (vehicle->object.damage_flags & (1u << _object_dead_bit))
        return 0;

    if (scripted_request)
        return 1;

    float distance_threshold = already_attempting_entry ? attempt_continue_distance : attempt_start_distance;

    real_point3d vehicle_origin;
    object_get_origin(vehicle_index, &vehicle_origin);

    real_point3d *actor_position = &actor->input.position.body_position;
    float dx = vehicle_origin.n[0] - actor_position->n[0];
    float dy = vehicle_origin.n[1] - actor_position->n[1];
    float dz = vehicle_origin.n[2] - actor_position->n[2];

    unsigned __int8 desirable = 0;

    if (already_inside || dx * dx + dz * dz + dy * dy < distance_threshold * distance_threshold)
    {
        desirable = 1;

        if (!already_inside && !already_attempting_entry)
        {
            const real_vector3d *velocity = &vehicle->object.translational_velocity;
            float speed_squared = velocity->n[0] * velocity->n[0] + velocity->n[2] * velocity->n[2]
                    + velocity->n[1] * velocity->n[1];
            if (speed_squared > 0.00027777778f)
                desirable = 0;
        }
    }

    if (already_inside)
        return desirable;

    if (vehicle->object.up.n[2] >= 0.5f)
        return desirable;

    return 0;
}
