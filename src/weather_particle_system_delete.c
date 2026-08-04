/* weather_particle_system_delete @0x8373C808 — tear down one active weather particle system: walk each
 * particle type's linked list of particles (weather_particle_datum.next_particle_index), free each
 * particle, then mark the system slot free. */

#include <stdint.h>
#include "headers/weather_particle_system_globals.h"
#include "headers/weather_particle_system_definition.h"
#include "headers/weather_particle_datum.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/blam_data_globals.h"

extern void datum_delete(data_array *data, int index);

void weather_particle_system_delete(int16_t system_index)
{
    weather_particle_system_datum *system = &weather_particle_system_globals.systems[system_index];
    weather_particle_system_definition *definition =
        TAG_GET(weather_particle_system_definition, system->definition_index);

    int type_count = definition->particle_types.count;
    for (int type = 0; type < type_count; type = (__int16)(type + 1))
    {
        weather_particle_type_datum *type_datum = &system->types[type];
        if (type_datum->first_particle_index != -1)
        {
            int next;
            do
            {
                int particle_index = type_datum->first_particle_index;
                weather_particle_datum *particle =
                    (weather_particle_datum *)weather_particle_data->data + (unsigned __int16)particle_index;
                next = particle->next_particle_index;
                datum_delete(weather_particle_data, particle_index);
                __int16 particle_count = type_datum->particle_count;
                type_datum->first_particle_index = next;
                type_datum->particle_count = particle_count - 1;
            } while (next != -1);
        }
    }

    --weather_particle_system_globals.active_system_count;
    system->definition_index = -1;
}
