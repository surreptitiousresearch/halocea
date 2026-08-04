/* weather_particle_systems_dispose_from_old_map @ 0x8373C088 — invalidate weather pool */
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"
extern void data_make_invalid(data_array *data);
void weather_particle_systems_dispose_from_old_map(void)
{
    if ( weather_particle_data->valid )
        data_make_invalid(weather_particle_data);
}
