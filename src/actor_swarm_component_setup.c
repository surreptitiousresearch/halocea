/* actor_swarm_component_setup @0x8371FED8 — register a unit/component pair in a swarm cache: appends the unit
 * index and component index to the swarm's parallel arrays (counted by word 1), clears the component's link
 * field, and refreshes the component from the unit. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/swarm_datum.h"
#include "headers/swarm_component_datum.h"
#include "headers/blam_data_globals.h"

extern void actor_swarm_component_refresh(int unit_index, uint16_t swarm_component_index);

void actor_swarm_component_setup(uint16_t swarm_index, int unit_index, uint16_t swarm_component_index)
{
    swarm_datum *swarm = DATA_ARRAY_ELEMENT(swarm_data, swarm_datum, swarm_index);
    DATA_ARRAY_ELEMENT(swarm_component_data, swarm_component_datum, swarm_component_index)->combat_target_prop_index = -1;
    swarm->unit_indices[swarm->unit_count] = unit_index;
    swarm->component_indices[swarm->unit_count++] = swarm_component_index;
    actor_swarm_component_refresh(unit_index, swarm_component_index);
}
