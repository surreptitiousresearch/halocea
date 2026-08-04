/* particle_systems_initialize_for_new_map @0x83739B90 — revalidate the particle-system and per-particle pools. */

#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void data_make_valid(data_array *data);

void particle_systems_initialize_for_new_map(void)
{
    data_make_valid(particle_systems);
    data_make_valid(system_particles);
}
