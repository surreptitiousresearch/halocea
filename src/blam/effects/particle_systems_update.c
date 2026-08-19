/* particle_systems_update @0x8373BC20 — ticks every live particle system this frame. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"


#include "headers/data_array.h"
extern int data_next_index(const data_array *data, int16_t index);
extern void particle_system_update(float dtime, int particle_system_index);

void particle_systems_update(float dtime)
{
    int index;

    for ( index = data_next_index(particle_systems, -1); index != -1;
          index = data_next_index(particle_systems, index) )
    {
        particle_system_update(dtime, index);
    }
}
