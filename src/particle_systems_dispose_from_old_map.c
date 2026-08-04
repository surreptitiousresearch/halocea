/* particle_systems_dispose_from_old_map @ 0x8373B9C8 — for every live particle system,
 * walk its per-emitter particle linked lists and delete each system particle, then the
 * system datum; finally invalidate both pools. */
#include <stdint.h>
#include "headers/data_array.h"
#include "headers/particle_system_datum.h"
#include "headers/particle_system_definition.h"
#include "headers/ps_particle_datum.h"
#include "headers/data_array.h"
extern int data_next_index(const data_array *data, int16_t index);
extern void datum_delete(data_array *data, int index);
extern void data_make_invalid(data_array *data);
#include "headers/global_tag_instances.h"  /* global_tag_instances; base_address@0x14 (tag-instance table) */
#include "headers/blam_data_globals.h"
void particle_systems_dispose_from_old_map(void)
{
    int i;
    if ( !particle_systems || !particle_systems->valid )
        return;

    for ( i = data_next_index(particle_systems, -1); i != -1; i = data_next_index(particle_systems, i) )
    {
        particle_system_datum *system = DATUM_GET(particle_systems, particle_system_datum, i);
        particle_system_definition *tag_def =
            TAG_GET(particle_system_definition, system->definition_index);
        if ( tag_def->types.count > 0 )
        {
            int e;
            for ( e = 0; e < tag_def->types.count; ++e )
            {
                int p = system->types[e].first_particle_index;
                while ( p != -1 )
                {
                    int next = DATUM_GET(system_particles, ps_particle_datum, p)->next_particle_index;
                    datum_delete(system_particles, p);
                    p = next;
                }
            }
        }
        datum_delete(particle_systems, i);
    }
    data_make_invalid(particle_systems);
    data_make_invalid(system_particles);
}
