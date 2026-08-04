/* actor_perception_acknowledge @0x837D5448 — clear a prop's pending-search/uncover flags, mark it for a
 * stimulus refresh, and fire the prop-acknowledged stimulus on the actor. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/prop_datum.h"
#include "headers/blam_data_globals.h"

extern void actor_stimulus_prop_acknowledged(int actor_index, int prop_index, uint8_t reappearance, uint8_t expected);

void actor_perception_acknowledge(int actor_index, int prop_index, uint8_t had_orphan,
                                  uint8_t expected)
{
    prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, prop_index);
    prop->tried_to_search = 0;
    prop->tried_to_uncover = 0;
    prop->abandoned_search = 0;
    prop->refresh_stimuli = 1;
    actor_stimulus_prop_acknowledged(actor_index, prop_index, had_orphan, expected);
}
