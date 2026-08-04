/* weather_particle_systems_initialize @ 0x8373C088 — 512-entry, 84-byte weather particle pool */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern data_array *data_new(const char *name, int16_t maximum_count, int16_t size);

void weather_particle_systems_initialize(void)
{
    weather_particle_data = data_new("weather particles", 512, 84);
}
