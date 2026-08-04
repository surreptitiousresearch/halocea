#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void datum_delete(data_array *data, int index);

void particle_delete(int particle_index)
{
    datum_delete(particle_data, particle_index);
}
