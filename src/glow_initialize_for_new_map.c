#include "headers/glow_globals.h"

extern void data_make_valid(data_array *data);

void glow_initialize_for_new_map(void)
{
    if (glow_globals.glow_data)
        data_make_valid(glow_globals.glow_data);
    if (glow_globals.glow_particle_data)
        data_make_valid(glow_globals.glow_particle_data);
}
