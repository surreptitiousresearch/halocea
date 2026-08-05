/* action_uncover_setup_pursuit @0x83824840 — set up an "uncover" pursuit toward one of the actor's
 * encounter's firing positions: fails (returns 0, state left zeroed) if the actor is dead (byte +352),
 * has no combat form (byte +6), has no encounter (dword +52 == -1), or firing_position_index is -1.
 * Otherwise records the firing position's world data (scenario ai_encounters field +156, 24-byte stride)
 * into state_data->pursuit_location, marks it uninspected and searchable, and marks the actor as pursuing
 * (byte +152). Mirrors action_search_setup_pursuit.c's identical setup path. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/scenario.h"
#include "headers/uncover_state_data.h"
#include "headers/encounter_definition.h"
#include "headers/firing_position_definition.h"
#include "headers/pursuit_location_type.h"
#include "headers/blam_data_globals.h"

extern void *memset(void *destination, int value, unsigned int size);

uint8_t action_uncover_setup_pursuit(uint16_t actor_index, int16_t firing_position_index, uncover_state_data *state_data)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    memset(state_data, 0, sizeof(uncover_state_data));

    if ( actor->input.vehicle_passenger || actor->meta.swarm || actor->meta.encounter_index == -1 || firing_position_index == -1 )
        return 0;

    firing_position_definition *firing_positions =
        (firing_position_definition *)((encounter_definition *)global_scenario->ai_encounters.address)
        [(uint16_t)actor->meta.encounter_index].firing_positions.address;
    firing_position_definition *firing_position = &firing_positions[firing_position_index];

    state_data->pursuit_location.type = _pursuit_location_position;
    state_data->pursuit_location.firing_position_index = firing_position_index;
    state_data->pursuit_location.position.n[0] = firing_position->position.n[0];
    state_data->pursuit_location.position.n[1] = firing_position->position.n[1];
    state_data->pursuit_location.position.n[2] = firing_position->position.n[2];
    state_data->pursuit_location.surface_index = firing_position->surface_index;
    state_data->pursuit_location.cluster_index = firing_position->cluster_index;
    state_data->pursuit_location_inspected = 0;
    state_data->able_to_search = 1;

    actor->state.searching = 1;
    return 1;
}
