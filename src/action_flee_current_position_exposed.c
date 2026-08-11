/* action_flee_current_position_exposed @0x838255C0 — check whether the actor's chosen flee prop/firing
 * position is exposed to whatever it's fleeing from. Estimates the actor's body position (from the
 * scenario's cached firing-position point), then tests line of sight from there to the flee-target
 * prop's head position (ignoring the prop's own vehicle, and ignoring vehicles generally if the actor
 * itself is in one). If the sightline is a clean hit against a moving-target-class prop (state 2 or 3),
 * the position is considered exposed (return true). Either way, if the line-of-sight result is "clear"
 * or "grazing" (0 or 3), records the prop's head position as the flee state's approach point.
 *
 * DEVIATION: the trailing "mode" argument to ai_test_line_of_sight collapses under algebra to
 * (vehicle_index != -1) — the decompiler renders it as `byte[344] + 1 - (byte[344] + (dword@344==-1))`,
 * which is `1 - (dword@344==-1)` once the byte term cancels (the byte read has no effect on the result);
 * dword@344 is actor->input.vehicle_index. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/prop_datum.h"
#include "headers/scenario.h"
#include "headers/flee_state_data.h"
#include "headers/prop_perception_state.h"
#include "headers/encounter_definition.h"
#include "headers/firing_position_definition.h"
#include "headers/blam_data_globals.h"


#include "headers/real_vector3d.h"
extern void unit_estimate_position(int unit_index, int16_t estimate_mode, const real_point3d *body_position, real_vector3d *desired_facing, real_vector3d *desired_gun_offset, real_point3d *estimated_position);
extern int16_t ai_test_line_of_sight(const real_point3d *p0, int16_t p0_cluster_index, const real_point3d *p1, int16_t p1_cluster_index, int16_t mode, uint8_t test_line_of_fire, int ignore_object_index, uint8_t ignore_vehicles);

uint8_t action_flee_current_position_exposed(int actor_index, flee_state_data *flee_state_data)
{
    uint8_t exposed = 0;
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if ( flee_state_data->flee_prop_index == -1 || actor->meta.encounter_index == -1 )
        return exposed;

    int16_t firing_position_index = flee_state_data->flee_firing_position_index;
    if ( firing_position_index == -1 )
        return exposed;

    /* ai_encounters[encounter_index].firing_positions.address[firing_position_index]: the scenario's
     * encounter_definition block (176-byte stride) holds a firing_positions tag_block at +152; its .address
     * (+156) points at an array of firing_position_definition (24 bytes each). */
    const encounter_definition *encounter =
        &((const encounter_definition *)global_scenario->ai_encounters.address)[(uint16_t)actor->meta.encounter_index];
    const firing_position_definition *firing_position =
        &((const firing_position_definition *)encounter->firing_positions.address)[firing_position_index];

    prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, flee_state_data->flee_prop_index);

    real_point3d estimated_position;
    unit_estimate_position(actor->meta.unit_index, 2, &firing_position->position, nullptr, nullptr,
        &estimated_position);

    int16_t sight_result = ai_test_line_of_sight(&estimated_position, firing_position->cluster_index,
        &prop->head_position, prop->body_location.cluster_index, 1, 0,
        prop->vehicle_index, actor->input.vehicle_index != -1);

    if ( prop->state >= _prop_state_becoming_unacknowledged && prop->state <= _prop_state_acknowledged )
        exposed = (sight_result == 0);

    if ( !sight_result || sight_result == 3 )
    {
        flee_state_data->has_approach_point = 1;
        flee_state_data->approach_point = prop->head_position;
    }

    return exposed;
}
