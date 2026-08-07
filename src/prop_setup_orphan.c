/* prop_setup_orphan @0x837D19F0 — repurpose a prop slot as an "orphan" copy of another prop: copy the
 * source prop (orphan_prop_index) over the target prop slot but preserve the target's identity header
 * (identifier/owner/next/orphan-link, first 16 bytes), then reset the orphan's bookkeeping (state = 4
 * "orphan", lifespan 900 ticks, inspection 0, tried/abandoned-search flags cleared), point the orphan hint
 * vector from the last-perceived position to the current body position, zero the velocity, and clear the
 * quantized speed. 2026-07-13: fully typed (prop_datum); the first parameter is the TARGET prop index (the
 * prior transcription's `actor_index` name was wrong). acknowledged_prop_index is unused here (a stale
 * caller register). */

#include <stdint.h>
#include <string.h>
#include "headers/data_array.h"
#include "headers/prop_datum.h"
#include "headers/real_vector3d.h"
#include "headers/prop_perception_state.h"
#include "headers/blam_data_globals.h"


void prop_setup_orphan(uint16_t prop_index, uint16_t orphan_prop_index, int acknowledged_prop_index)
{
    prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, prop_index);

    int16_t identifier = prop->identifier;
    int owner_actor_index = prop->owner_actor_index;
    int next_prop_index = prop->next_prop_index;
    int orphan_link = prop->___u3.orphan_prop_index;

    memcpy(prop, DATA_ARRAY_ELEMENT(prop_data, prop_datum, orphan_prop_index), sizeof(*prop));

    prop->identifier = identifier;
    prop->owner_actor_index = owner_actor_index;
    prop->next_prop_index = next_prop_index;
    prop->___u3.orphan_prop_index = orphan_link;
    prop->state = _prop_state_uninspected_orphan;
    prop->orphan_lifespan_ticks = 900;
    prop->orphan_inspection_ticks = 0;
    prop->tried_to_uncover = 0;
    prop->tried_to_search = 0;
    prop->abandoned_search = 0;

    prop->orphan_hint_vector.n[0] = prop->body_position.__s1.x - prop->last_perceived_body_position.__s1.x;
    prop->orphan_hint_vector.n[1] = prop->body_position.__s1.y - prop->last_perceived_body_position.__s1.y;
    prop->orphan_hint_vector.n[2] = prop->body_position.__s1.z - prop->last_perceived_body_position.__s1.z;
    prop->velocity = *global_zero_vector3d;
    prop->quantized_speed = 0;
}
