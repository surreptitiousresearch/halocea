/* particles_initialize @ 0x8373D950 — 1024-entry, 112-byte particle pool */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern data_array *game_state_data_new(const char *name, int16_t maximum_count, int16_t size);

void particles_initialize(void)
{
    particle_data = game_state_data_new("particle", 1024, 112);
}
