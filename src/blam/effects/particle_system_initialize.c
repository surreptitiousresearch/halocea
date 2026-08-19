/* particle_system_initialize @0x8373BAD0 — bring a freshly allocated particle-system datum online: resolve
 * its scenario location, mark it active, and seed each emitter's per-emitter state from the system
 * definition. Returns nonzero only when every emitter is valid (v2) AND at least one emitter actually
 * initialized a particle (v3); on that success it advances the system one small step so it is non-empty.
 *
 * Uses particle_system_datum (per-emitter runtime = types[4]), particle_system_definition.types (emitters),
 * and each emitter's type_states[0] duration bounds for the initial random age. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/location.h"
#include "headers/real_point3d.h"
#include "headers/particle_system_datum.h"
#include "headers/particle_system_definition.h"
#include "headers/particle_system_type.h"
#include "headers/particle_system_type_state.h"
#include "headers/particle_system_flags.h"
#include "headers/blam_data_globals.h"

extern void scenario_location_from_point(location *location, const real_point3d *point);
extern uint32_t *get_global_local_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);
extern void particle_system_update(float dtime, int particle_system_index);

uint8_t particle_system_initialize(int new_system_index)
{
    particle_system_datum *system =
        DATA_ARRAY_ELEMENT(particle_systems, particle_system_datum, new_system_index);
    particle_system_definition *definition =
        TAG_GET(particle_system_definition, system->definition_index);
    int all_emitters_valid = 1;
    char any_particle_seeded = 0;
    int emitter_count;
    int result;

    scenario_location_from_point(&system->location, &system->position);
    system->flags |= (1u << _particle_system_initializing_bit);   /* mark initializing */

    emitter_count = definition->types.count;
    if ( emitter_count > 0 )
    {
        int i;
        for ( i = 0; i < emitter_count; i = (int16_t)(i + 1) )
        {
            particle_system_type *emitter =
                &((particle_system_type *)definition->types.address)[i];
            particle_type *emitter_state = &system->types[i];

            if ( emitter->type_states.count )
            {
                emitter_state->state_index = 0xFFFF;
                emitter_state->states_moving_forward = 1;
                emitter_state->particle_count = 0;
                emitter_state->first_particle_index = -1;
                if ( emitter->type_states.count > 0 )
                {
                    particle_system_type_state *particle_def =
                        (particle_system_type_state *)emitter->type_states.address;
                    float age_max = particle_def->duration_upper_bound;
                    float age_min = particle_def->duration_lower_bound;
                    float age = real_seed_random_range(get_global_local_random_seed_address(),
                            age_min, age_max);
                    emitter_state->time_left_in_state = age;
                    any_particle_seeded = 1;
                    emitter_state->state_length = age;
                }
            }
            else
            {
                all_emitters_valid = 0;
            }
        }
    }

    result = -(any_particle_seeded != 0) & all_emitters_valid;
    if ( ((unsigned char)-(any_particle_seeded != 0) & (unsigned char)all_emitters_valid) != 0 )
        particle_system_update(0.001f, new_system_index);   /* deviation: decompiler showed emitter_count as arg; disasm passes the system index */
    return result;
}
