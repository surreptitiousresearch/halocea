/* actor_reset_idle_vocalization_timer @0x837CBD08 — schedule the actor's next idle vocalization. Picks a random
 * delay within the character's combat or non-combat idle-vocalization range (character tag floats [254..255]
 * non-combat min/max, [256..257] combat min/max), offset by the unit's current speech delay, and stores it in
 * the actor's idle-vocalization timer (actor +0x6CE word). Also records whether the actor is in combat
 * (actor +0x6CC byte).
 *
 * Deviation: Hex-Rays widens the single-precision range math to double and packs the offset into a fused
 * __int64 slot; reconstructed as float/int. */

#include <stdint.h>
#include "headers/unit_datum.h"
#include "headers/actor_definition.h"
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/blam_data_globals.h"

extern uint8_t actor_in_combat(int actor_index);
extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);

void actor_reset_idle_vocalization_timer(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor_definition *character = TAG_GET(actor_definition, actor->meta.definition_index);
    uint8_t in_combat = actor_in_combat(actor_index);

    int16_t speech_offset = 0;
    if ( actor->meta.unit_index != -1 )
    {
        unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, actor->meta.unit_index)->datum);
        if ( unit->unit.speech.current.priority > 0 )
            speech_offset = (uint16_t)unit->unit.speech.sound_timer;
    }

    float min_delay, max_delay;
    if ( in_combat )
    {
        max_delay = character->communication.idle_combat_time_upper_bound;
        min_delay = character->communication.idle_combat_time_lower_bound;
    }
    else
    {
        max_delay = character->communication.idle_noncombat_time_upper_bound;
        min_delay = character->communication.idle_noncombat_time_lower_bound;
    }

    unsigned int *seed = get_global_random_seed_address();
    float delay = real_seed_random_range(seed, min_delay, max_delay);
    actor->control.idle_vocalization_combat = in_combat;
    actor->control.idle_vocalization_timer = (int)(delay * 30.0f + (float)speech_offset);
}
