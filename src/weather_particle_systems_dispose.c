/* weather_particle_systems_dispose @ 0x8373C088 — free the weather particle pool */
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"
#include "headers/data_array.h"
extern void data_dispose(data_array *data);
void weather_particle_systems_dispose(void)
{
    if ( weather_particle_data )
    {
        data_dispose(weather_particle_data);
        weather_particle_data = 0;
    }
}
