#include "headers/data_array.h"
#include "headers/glow_datum.h"
#include "headers/glow_particle.h"
#include "headers/glow_globals.h"

extern int datum_new(data_array *data);
extern void *datum_get(data_array *data, int index);

glow_particle * glow_particle_new(glow_datum *glow)
{
    int index = datum_new(glow_globals.glow_particle_data);
    if (index == -1)
        return nullptr;

    glow_particle *particle = datum_get(glow_globals.glow_particle_data, index);
    particle->index = index;
    return particle;
}
