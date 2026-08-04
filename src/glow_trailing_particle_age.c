/* glow_trailing_particle_age @0x8380B840 — ages a glow's trailing particle: once past its lifetime, unlinks
 * it from the glow's doubly-linked particle list and deletes its datum. */

#include "headers/glow_datum.h"
#include "headers/glow_particle.h"
#include "headers/glow_globals.h"
#include "headers/data_array.h"

extern void datum_delete(data_array *data, int index);

void glow_trailing_particle_age(glow_datum *glow, glow_particle *particle)
{
    if ( particle->ticks_in_existence > particle->lifetime )
    {
        glow_particle *previous = particle->previous;
        glow_particle *next = particle->next;

        if ( previous )
            previous->next = next;
        else
            glow->head_particle = next;

        if ( next )
            next->previous = previous;
        else
            glow->tail_particle = previous;

        datum_delete(glow_globals.glow_particle_data, particle->index);
        --glow->number_of_particles;
    }
}
