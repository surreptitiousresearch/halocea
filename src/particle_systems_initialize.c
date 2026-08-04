/* particle_systems_initialize @ 0x83739B38 — particle-system + system-particle pools */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern data_array *game_state_data_new(const char *name, int16_t maximum_count, int16_t size);

void particle_systems_initialize(void)
{
    particle_systems = game_state_data_new("particle systems", 64, 344);
    system_particles = game_state_data_new("particle system particles", 512, 128);
}
