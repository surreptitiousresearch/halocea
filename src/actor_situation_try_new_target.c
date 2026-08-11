/* actor_situation_try_new_target @0x837D7B38 — considers switching the actor's targeted prop to `prop_index`.
 * Computes the candidate's target weight and stores it in the prop's own record
 * (prop_datum.target_weight @80, DB-resolved); refuses if the candidate scores non-positive or
 * scores below the actor's current target's weight. Otherwise adopts it: sets the new target prop index,
 * resets the target-related timer, clears the firing position, and refreshes target/combat status. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/actor_target_type.h"
#include "headers/prop_datum.h"
#include "headers/blam_data_globals.h"

extern float actor_compute_prop_target_weight(int actor_index, int prop_index);
extern void actor_situation_update_target_status(int actor_index);
extern void actor_situation_combat_status_update(int actor_index);

uint8_t actor_situation_try_new_target(int actor_index, int prop_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    prop_datum *props = (prop_datum *)prop_data->data;

    prop_datum *candidate_prop = &props[(uint16_t)prop_index];

    int current_target_prop_index = actor->target.target_prop_index;
    prop_datum *current_target_prop = (current_target_prop_index == -1)
            ? 0
            : &props[(uint16_t)current_target_prop_index];

    float weight = actor_compute_prop_target_weight(actor_index, prop_index);
    candidate_prop->target_weight = weight;   /* +80, DB prop_datum.target_weight */

    if (weight <= 0.0f || (current_target_prop && weight < current_target_prop->target_weight))
        return 0;

    actor->target.target_prop_index = prop_index;
    actor->target.target_type = actor_target_none;
    actor->target.target_last_visible_time = -1;

    actor_situation_update_target_status(actor_index);
    actor_situation_combat_status_update(actor_index);
    return 1;
}
