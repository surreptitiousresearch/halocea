/* particle_get_radius @0x8373DD28 — lerp the particle's definition-tuned initial/final radius modifiers by
 * its lifetime fraction, scaled by the particle's own base radius. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/particle_datum.h"
#include "headers/particle_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/blam_data_globals.h"


#include "headers/data_array.h"
extern void *datum_get(data_array *data, int index);

float particle_get_radius(int16_t particle_index)
{
    particle_datum *particle = datum_get(particle_data, particle_index);
    particle_definition *definition = TAG_GET(particle_definition, particle->definition_index);
    float life_fraction = particle->life_time / particle->life_span;

    return (life_fraction * (definition->final_radius_modifier - definition->initial_radius_modifier)
            + definition->initial_radius_modifier) * particle->radius;
}
