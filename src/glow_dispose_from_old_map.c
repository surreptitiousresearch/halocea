#include "headers/glow_globals.h"

extern void data_make_invalid(data_array *data);

void glow_dispose_from_old_map(void)
{
    if (glow_globals.glow_data)
        data_make_invalid(glow_globals.glow_data);
    if (glow_globals.glow_particle_data)
        data_make_invalid(glow_globals.glow_particle_data);
}
