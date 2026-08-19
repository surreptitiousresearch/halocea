/* glow_delete @0x8380B590 */
#include "headers/glow_globals.h"
#include "headers/glow_datum.h"
#include "headers/glow_particle.h"

extern void *datum_get(data_array *data, int index);
extern void datum_delete(data_array *data, int index);

void glow_delete(int glow_index)
{
    glow_datum *glow = datum_get(glow_globals.glow_data, glow_index);
    glow_particle *particle = glow->head_particle;

    while ( particle )
    {
        glow_particle *next = particle->next;

        datum_delete(glow_globals.glow_particle_data, particle->index);
        particle = next;
    }
    datum_delete(glow_globals.glow_data, glow_index);
}
