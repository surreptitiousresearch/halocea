/* particles_dispose @ 0x8373D9A8 — drop the particle pool pointer */
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"
void particles_dispose(void)
{
    if ( particle_data )
        particle_data = 0;
}
