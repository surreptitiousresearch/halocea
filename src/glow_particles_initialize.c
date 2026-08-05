/* glow_particles_initialize @0x8380C980 — allocate and link a glow's full particle chain: creates
 * number_of_particles particles (stopping early if allocation fails), applying the glow definition's flag
 * bit 0x2 (constant flag +1) and bit 0x4 (alternating flag +1, toggled per particle) to each, then links
 * them into the glow's head/tail particle list in creation order. */

#include <stdint.h>
#include "headers/glow_datum.h"
#include "headers/glow_particle.h"
#include "headers/glow_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/glow_definition_flags.h"
#include "headers/glow_particle_flags.h"

extern glow_particle * glow_normal_particle_new(glow_datum *glow, int16_t index, int16_t total);

void glow_particles_initialize(glow_datum *glow)
{
    uint8_t alternate = 1;
    glow_particle *previous = 0;
    glow_definition *definition = TAG_GET(glow_definition, glow->definition_index);

    for ( int16_t index = 0; index < glow->number_of_particles; index++ )
    {
        glow_particle *particle = glow_normal_particle_new(glow, index, glow->number_of_particles);
        if ( !particle )
            break;

        if ( (definition->flags & (1u << _glow_particles_initial_direction_negative_bit)) != 0 )
            particle->flags |= (1u << _glow_particle_traversal_direction_negative_bit);

        if ( (definition->flags & (1u << _glow_particles_move_in_both_directions_bit)) != 0 )
        {
            particle->flags = alternate
                    ? (particle->flags & ~(1u << _glow_particle_traversal_direction_negative_bit))
                    : (particle->flags | (1u << _glow_particle_traversal_direction_negative_bit));
            alternate = !alternate;
        }

        if ( !glow->head_particle )
            glow->head_particle = particle;
        if ( previous )
            previous->next = particle;
        particle->previous = previous;
        glow->tail_particle = particle;
        previous = particle;
    }
}
