/* actors_initialize @ 0x8371CCB0 — actor type table + actor/swarm pools */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void actor_types_initialize(void);
extern data_array *game_state_data_new(const char *name, int16_t maximum_count, int16_t size);

void actors_initialize(void)
{
    actor_types_initialize();
    actor_data = game_state_data_new("actor", 256, 1828);
    swarm_data = game_state_data_new("swarm", 32, 152);
    swarm_component_data = game_state_data_new("swarm component", 256, 64);
}
