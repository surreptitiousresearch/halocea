/* particles_dispose_from_old_map @ 0x8373D998 — invalidate particle pool */
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"
extern void data_make_invalid(data_array *data);
void particles_dispose_from_old_map(void)
{
    data_make_invalid(particle_data);
}
