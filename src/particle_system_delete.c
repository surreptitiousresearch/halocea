/* particle_system_delete @0x8373A250 — free a particle system and all particles owned by each of its
 * emitter types, then free the system datum itself. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/particle_system_datum.h"
#include "headers/particle_system_definition.h"
#include "headers/ps_particle_datum.h"
#include "headers/blam_data_globals.h"


extern void datum_delete(data_array *data, int index);

void particle_system_delete(int system_index)
{
    data_array *array = particle_systems;
    particle_system_datum *system = DATUM_GET(particle_systems, particle_system_datum,
                                                       system_index);
    particle_system_definition *definition =
        TAG_GET(particle_system_definition, system->definition_index);
    int type;

    if ( definition->types.count > 0 )
    {
        for ( type = 0; type < definition->types.count; type = (int16_t)(type + 1) )
        {
            int particle = system->types[type].first_particle_index;
            while ( particle != -1 )
            {
                int next = DATUM_GET(system_particles, ps_particle_datum,
                                              particle)->next_particle_index;
                datum_delete(system_particles, particle);
                particle = next;
            }
        }
        array = particle_systems;
    }
    datum_delete(array, system_index);
}
