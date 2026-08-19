/* particle_die @0x8373DD80 — destroy a particle. If its definition specifies a death-particle effect
 * (death_effect.index != -1), spawn that effect at the dying particle, then free the datum. */

#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/particle_datum.h"
#include "headers/particle_definition.h"
#include "headers/blam_data_globals.h"


extern void particle_effect_new(particle_datum *particle, unsigned int group_tag, int effect_index, float scale);
extern void datum_delete(data_array *data, int index);
extern void *datum_get(data_array *data, int index);

void particle_die(int particle_index)
{
    data_array *array = particle_data;
    particle_datum *particle = datum_get(particle_data, particle_index);
    particle_definition *definition = TAG_GET(particle_definition, particle->definition_index);
    int death_effect_index = definition->death_effect.index;

    if ( death_effect_index != -1 )
    {
        particle_effect_new(particle, definition->death_effect.group_tag, death_effect_index, 0.0);
        array = particle_data;
    }
    datum_delete(array, particle_index);
}
