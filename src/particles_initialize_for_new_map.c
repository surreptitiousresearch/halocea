/* particles_initialize_for_new_map @0x8373D988 — reset the particle pool. */

#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void data_make_valid(data_array *data);

void particles_initialize_for_new_map(void)
{
    data_make_valid(particle_data);
}
