/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* weather_particle_system_new_particle @0x8373C250 — spawn one particle for a weather system's given type
 * (sibling of weather_particle_system_update). Pulls a fresh particle datum (returns -1 on pool exhaustion) and
 * randomizes it from the weather tag's per-type definition: a spawn offset within the type's box, a random unit
 * travel direction scaled by a random speed, three random animation parameters, a sway phase (random over 2pi
 * only when the type's "animated sway" flag is set), a random phase index into the type-bitmap's period table and
 * a random phase within that period, an interpolated colour, and a random fade value. The particle is then linked
 * at the head of the type's live-particle list and the type's particle count is bumped. Returns the datum index.
 *
 * The system's per-type state (Hex-Rays' `&system->definition_index + 4*type_index` read at +32/+36/+40) is just
 * &system->types[type_index] (the -0x1C base cancels against types[]'s 0x1C offset). The weather tag definition,
 * its per-type sub-block (stride 604), and the type-bitmap tag are read at raw offsets; the 84-byte particle
 * datum fields are written at raw offsets as in the update sibling. */

#include <stdint.h>
#include "headers/weather_particle_system_globals.h"
#include "headers/weather_particle_datum.h"
#include "headers/weather_particle_type_definition.h"
#include "headers/weather_particle_system_definition.h"
#include "headers/bitmap_group.h"
#include "headers/bitmap_group_sequence.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/real_vector3d.h"
#include "headers/real_rgb_color.h"
#include "headers/weather_particle_type_flags.h"
#include "headers/blam_data_globals.h"
#include "headers/math_constants.h"


extern int datum_new(data_array *data);
extern uint32_t *get_global_local_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);
extern float real_seed_random(uint32_t *seed);
extern int16_t seed_random_range(uint32_t *seed, int16_t lower_bound, int16_t upper_bound);
extern real_vector3d *seed_random_direction3d(uint32_t *seed, real_vector3d *direction);
extern real_rgb_color *rgb_colors_interpolate(real_rgb_color *rgb_result, unsigned int flags, const real_rgb_color *rgb_lower_bound, const real_rgb_color *rgb_upper_bound, float u);

int weather_particle_system_new_particle(int16_t system_index, int16_t type_index)
{
    int particle_index = datum_new(weather_particle_data);
    if ( particle_index == -1 )
        return -1;

    weather_particle_system_datum *system = &weather_particle_system_globals.systems[system_index];
    weather_particle_type_datum *type_state = &system->types[type_index];
    float box_width = type_state->box_width;

    weather_particle_datum *particle = DATA_ARRAY_ELEMENT(weather_particle_data, weather_particle_datum, particle_index);

    weather_particle_system_definition *definition = TAG_GET(weather_particle_system_definition, system->definition_index);
    weather_particle_type_definition *type_def = (weather_particle_type_definition *)definition->particle_types.address + type_index;
    bitmap_group *type_tag = TAG_GET(bitmap_group, type_def->bitmap.index);

    unsigned int *seed;

    seed = get_global_local_random_seed_address();
    particle->position.n[0] = real_seed_random_range(seed, 0.0f, box_width);
    seed = get_global_local_random_seed_address();
    particle->position.n[1] = real_seed_random_range(seed, 0.0f, box_width);
    seed = get_global_local_random_seed_address();
    particle->position.n[2] = real_seed_random_range(seed, 0.0f, box_width);

    particle->velocity.n[2] = 0.0f;
    particle->velocity.n[1] = 0.0f;
    particle->velocity.n[0] = 0.0f;

    seed = get_global_local_random_seed_address();
    seed_random_direction3d(seed, &particle->acceleration);

    seed = get_global_local_random_seed_address();
    float speed = real_seed_random_range(seed, type_def->acceleration_lower_bound, type_def->acceleration_upper_bound);
    particle->acceleration.n[0] *= speed;
    particle->acceleration.n[1] *= speed;
    particle->acceleration.n[2] *= speed;

    seed = get_global_local_random_seed_address();
    particle->radius = real_seed_random_range(seed, type_def->radius_lower_bound, type_def->radius_upper_bound);
    seed = get_global_local_random_seed_address();
    particle->animation_rate = real_seed_random_range(seed, type_def->animation_rate_lower_bound, type_def->animation_rate_upper_bound);
    seed = get_global_local_random_seed_address();
    particle->rotation_rate = real_seed_random_range(seed, type_def->rotation_rate_lower_bound, type_def->rotation_rate_upper_bound);

    if ( (type_def->flags & (1u << _weather_particle_type_random_rotation_bit)) != 0 )
    {
        seed = get_global_local_random_seed_address();
        particle->rotation = real_seed_random_range(seed, 0.0f, TWO_PI);
    }
    else
    {
        particle->rotation = 0.0f;
    }

    int16_t period_count = type_tag->sequences.count;
    seed = get_global_local_random_seed_address();
    int16_t phase_index = seed_random_range(seed, 0, period_count);
    particle->sequence_index = phase_index;

    /* period = sequence[phase_index].sprites.count (bitmap_group_sequence stride 64, sprites count@+52) */
    int period = ((bitmap_group_sequence *)type_tag->sequences.address)[phase_index].sprites.count;
    seed = get_global_local_random_seed_address();
    particle->sprite_index = real_seed_random_range(seed, 0.0f, (float)period);

    seed = get_global_local_random_seed_address();
    float color_fraction = real_seed_random(seed);
    rgb_colors_interpolate((real_rgb_color *)&particle->color.n[1], type_def->flags,
            (const real_rgb_color *)&type_def->color_lower_bound.n[1], (const real_rgb_color *)&type_def->color_upper_bound.n[1], color_fraction);

    seed = get_global_local_random_seed_address();
    particle->color.n[0] = real_seed_random_range(seed, type_def->color_lower_bound.n[0],
            type_def->color_upper_bound.n[0]);

    particle->next_particle_index = type_state->first_particle_index;
    type_state->first_particle_index = particle_index;
    ++type_state->particle_count;

    return particle_index;
}
