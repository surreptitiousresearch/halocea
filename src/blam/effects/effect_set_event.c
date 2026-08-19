/* effect_set_event @0x836E1648 — switch an effect to a given event in its timeline: stamps the event
 * index, resets the event time, clears the "started" flag, and rolls a fresh randomized event duration
 * from that event's delay range. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/effect_datum.h"
#include "headers/effect_definition.h"
#include "headers/effect_event_definition.h"
#include "headers/effect_flags.h"
#include "headers/effect_definition_flags.h"
#include "headers/blam_data_globals.h"


extern void *datum_try_and_get(const data_array *data, int index);
extern uint32_t *get_global_random_seed_address(void);
extern uint32_t *get_global_local_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);

void effect_set_event(int effect_index, int16_t event_index)
{
    effect_datum *effect = datum_try_and_get(effect_data, effect_index);
    effect_definition *definition;
    effect_event_definition *event;
    uint32_t *seed;

    if ( !effect )
        return;

    definition = TAG_GET(effect_definition, effect->definition_index);
    if ( event_index < 0 || event_index >= definition->events.count )
        return;

    event = (effect_event_definition *)definition->events.address + event_index;
    effect->event_index = event_index;
    effect->event_time = 0.0f;
    effect->flags &= ~(1u << _effect_in_duration_bit);   /* clear "started" flag */

    seed = (TAG_GET(effect_definition, effect->definition_index)->flags & (1u << _effect_must_be_deterministic_bit)) != 0
        ? get_global_random_seed_address()
        : get_global_local_random_seed_address();
    effect->event_duration = real_seed_random_range(seed, event->delay_lower_bound, event->delay_upper_bound);
}
