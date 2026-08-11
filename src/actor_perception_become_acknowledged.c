/* actor_perception_become_acknowledged @0x837D8948 — promote a prop to the "acknowledged" state (3). If it
 * has an orphan companion prop, fold the orphan's persistence fields into this prop, repoint the actor's
 * references off the orphan and delete it. Then clear the search flags and fire the acknowledged stimulus.
 * Returns 1 if the promotion happened (state was not already in the 2..3 range). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/prop_datum.h"
#include "headers/prop_perception_state.h"
#include "headers/blam_data_globals.h"

extern int actor_expected_acknowledgement(int actor_index, int prop_index);
extern void actor_switch_props(int actor_index, int prop_index, int replacement_prop_index);
extern void prop_delete(int actor_index, int prop_index);
extern void actor_stimulus_prop_acknowledged(int actor_index, int prop_index, uint8_t reappearance, uint8_t expected);

uint8_t actor_perception_become_acknowledged(int actor_index, int prop_index,
                                                     uint8_t *expected_reference)
{
    prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, prop_index);
    uint8_t result = 0;
    uint8_t expected = 0;

    int16_t state = prop->state;
    if ( state < _prop_state_becoming_unacknowledged || state > _prop_state_acknowledged )
    {
        uint8_t had_orphan = (prop->___u3.orphan_prop_index != -1);
        expected = actor_expected_acknowledgement(actor_index, prop_index);
        if ( had_orphan )
        {
            int orphan_index = prop->___u3.orphan_prop_index;
            prop_datum *orphan = DATA_ARRAY_ELEMENT(prop_data, prop_datum, orphan_index);
            prop->target_weight = orphan->target_weight;
            prop->look_interest = orphan->look_interest;
            prop->last_idle_look_interest = orphan->last_idle_look_interest;
            prop->last_idle_look_time = orphan->last_idle_look_time;
            prop->unreachable_ticks = orphan->unreachable_ticks;
            prop->last_unreachable_time = orphan->last_unreachable_time;
            prop->unopposable_enemy = orphan->unopposable_enemy;
            prop->unopposable_casualties_inflicted = orphan->unopposable_casualties_inflicted;
            prop->unopposable_casualty_decay_timer = orphan->unopposable_casualty_decay_timer;
            actor_switch_props(actor_index, orphan_index, prop_index);
            prop_delete(actor_index, prop->___u3.orphan_prop_index);
            prop->___u3.orphan_prop_index = -1;
        }
        prop->state = _prop_state_acknowledged;
        prop->tried_to_search = 0;
        prop->tried_to_uncover = 0;
        prop->abandoned_search = 0;
        prop->refresh_stimuli = 1;
        actor_stimulus_prop_acknowledged(actor_index, prop_index, had_orphan, expected);
        result = 1;
    }
    if ( expected_reference )
        *expected_reference = expected;
    return result;
}
