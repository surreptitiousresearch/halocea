/* weather_particle_system_new @0x8373C108 — activate a weather particle system in a slot: record the
 * definition/scale, reset its timers, and initialise each particle type (empty particle list, zero count,
 * a randomized density within the definition's range, and the definition's box width). */

#include <stdint.h>
#include "headers/weather_particle_system_globals.h"
#include "headers/weather_particle_system_definition.h"
#include "headers/weather_particle_type_definition.h"
#include "headers/global_tag_instances.h"

extern uint32_t *get_global_local_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);

void weather_particle_system_new(int16_t system_index, int definition_index, float scale)
{
    weather_particle_system_datum *system = &weather_particle_system_globals.systems[system_index];
    weather_particle_system_definition *definition =
        TAG_GET(weather_particle_system_definition, definition_index);

    system->definition_index = definition_index;
    system->scale = scale;
    system->time = 0.0f;
    system->time_delta_sec = 0.0f;
    ++weather_particle_system_globals.active_system_count;

    int type_count = definition->particle_types.count;
    weather_particle_type_definition *type_defs =
        (weather_particle_type_definition *)definition->particle_types.address;
    for (int type = 0; type < type_count; type = (int16_t)(type + 1))
    {
        weather_particle_type_datum *type_datum = &system->types[type];
        type_datum->first_particle_index = -1;
        type_datum->particle_count = 0;

        weather_particle_type_definition *type_def = &type_defs[type];
        float density_max = type_def->particle_count_upper_bound;
        float density_min = type_def->particle_count_lower_bound;
        unsigned int *seed = get_global_local_random_seed_address();
        type_datum->density = real_seed_random_range(seed, density_min, density_max);
        /* box wrap width reuses the type's distance_fadeout_end (disasm 0x8373C1C4: lfs f0,0x30(type_def)) */
        type_datum->box_width = type_def->distance_fadeout_end;
    }
}
