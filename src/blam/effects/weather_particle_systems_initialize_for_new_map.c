/* weather_particle_systems_initialize_for_new_map @0x8373C0C0 — mark both weather slots inactive and
 * revalidate the weather-particle pool. */

#include "headers/weather_particle_system_globals.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void data_make_valid(data_array *data);

void weather_particle_systems_initialize_for_new_map(void)
{
    for ( int i = 0; i < 2; ++i )
        weather_particle_system_globals.systems[i].definition_index = -1;
    weather_particle_system_globals.active_system_count = 0;
    data_make_valid(weather_particle_data);
}
