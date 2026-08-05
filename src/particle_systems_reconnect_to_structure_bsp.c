/* particle_systems_reconnect_to_structure_bsp @0x8373A318 — after a structure BSP swap, recompute every
 * live particle system's cached BSP location from its cached position (or from its host object's current
 * location, if it's attached to one), deleting any particle system whose location comes back invalid
 * (no cluster). For particle systems that survive, walk each of the 4 emitter types' particle lists and
 * likewise recompute/validate each particle's own location, deleting particles that land outside the BSP
 * (splicing them out of their type's singly-linked list). max_particles_per_type is the count of the
 * particle system definition's `types` tag_block (@+92). Returns whatever data_next_index() last
 * returned (-1 once the particle_systems array is exhausted). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/particle_system_datum.h"
#include "headers/particle_system_definition.h"
#include "headers/ps_particle_datum.h"
#include "headers/blam_data_globals.h"


#include "headers/data_array.h"
extern int data_next_index(const data_array *data, int16_t index);
extern void scenario_location_from_point(location *location, const real_point3d *point);
extern location *object_get_location(int object_index, location *location_out);
extern void particle_system_delete(int system_index);
extern void datum_delete(data_array *data, int index);

int particle_systems_reconnect_to_structure_bsp(void)
{
    int system_index = data_next_index(particle_systems, -1);

    while ( system_index != -1 )
    {
        particle_system_datum *system = DATA_ARRAY_ELEMENT(particle_systems, particle_system_datum, system_index);

        int object_index = system->object_index;
        const particle_system_definition *definition =
            TAG_GET(const particle_system_definition, system->definition_index);

        if ( object_index != -1 )
        {
            object_get_location(object_index, &system->location);
        }
        else
        {
            scenario_location_from_point(&system->location, &system->position);
            if ( system->location.cluster_index == (int16_t)0xFFFF )
            {
                particle_system_delete(system_index);
                system_index = data_next_index(particle_systems, system_index);
                continue;
            }
        }

        int max_particles_per_type = definition->types.count;
        for ( int16_t type_index = 0; type_index < max_particles_per_type; ++type_index )
        {
            int *particle_index_ref = &system->types[type_index].first_particle_index;

            while ( *particle_index_ref != -1 )
            {
                /* recovered: (char*)data + ((idx<<7)&0x7FFF80) -> typed element at datum absolute index (stride 128 == sizeof(ps_particle_datum), unchecked) */
                ps_particle_datum *particle = DATA_ARRAY_ELEMENT(system_particles, ps_particle_datum, *particle_index_ref);

                scenario_location_from_point(&particle->location, &particle->position);

                if ( particle->location.cluster_index == (int16_t)0xFFFF )
                {
                    datum_delete(system_particles, *particle_index_ref);
                    *particle_index_ref = particle->next_particle_index;
                }
                else
                {
                    particle_index_ref = &particle->next_particle_index;
                }
            }
        }

        system_index = data_next_index(particle_systems, system_index);
    }

    return system_index;
}
