/* glow_trailing_particle_new @0x8380C6F0 — spawn one particle for a glow effect's trailing streak (sibling of
 * glow_normal_particle_new). Pulls a fresh glow_particle datum (null on pool exhaustion) and back-links its
 * index, then seeds its state from the glow tag definition: the spawn position (the sole marker's transform for
 * a single-marker glow, otherwise a random point along the emission length via get_particle_world_position); the
 * initial velocity direction (from the parent marker's matrix, a random unit vector, or straight "up" by the
 * definition's velocity mode) scaled by the definition's speed * 1/30; a random initial size divided by the
 * glow's bitmap dimension; the lifetime in ticks; and a colour lerped across the definition's two colour
 * endpoints (alpha forced to 1). Flag bit 0x2 is set. Returns the new particle.
 *
 * The glow tag definition fields are resolved to the DB glow_definition struct; glow_particle fields use the DB names. The
 * marker-velocity read reproduces the decompiler's `&glow->identifier + 54*parent_marker_index` form (108-byte
 * marker stride; +92/+96/+100 lands on that marker's matrix direction row). */

#include "headers/data_array.h"
#include "headers/glow_datum.h"
#include "headers/glow_particle.h"
#include "headers/glow_globals.h"
#include "headers/glow_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/real_vector3d.h"
#include "headers/glow_particle_flags.h"
#include "headers/blam_data_globals.h"
#include "headers/game_time_constants.h"
#include <stdint.h>


extern int datum_new(data_array *data);
extern void *datum_get(data_array *array, int index);
extern uint32_t *get_global_local_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);
extern float normalize3d(real_vector3d *v);
extern void get_particle_world_position(glow_datum *glow, glow_particle *particle, float rotation_per_unit_t);

glow_particle * glow_trailing_particle_new(glow_datum *glow)
{
    glow_definition *definition = TAG_GET(glow_definition, glow->definition_index);

    int particle_index = datum_new(glow_globals.glow_particle_data);
    if ( particle_index == -1 )
        return nullptr;

    glow_particle *particle = datum_get(glow_globals.glow_particle_data, particle_index);
    particle->index = particle_index;

    if ( glow->number_of_markers <= 1 )
    {
        particle->position.n[0] = glow->markers[0].matrix.position.n[0];
        particle->position.n[1] = glow->markers[0].matrix.position.n[1];
        particle->position.n[2] = glow->markers[0].matrix.position.n[2];
    }
    else
    {
        unsigned int *seed = get_global_local_random_seed_address();
        particle->t = real_seed_random_range(seed, definition->trailing_particle_minimum_normalized_t * glow->total_time,
                definition->trailing_particle_maximum_normalized_t * glow->total_time);
        get_particle_world_position(glow, particle, 0.0f);
    }

    uint16_t velocity_mode = definition->trailing_particle_distribution;
    if ( velocity_mode )
    {
        if ( velocity_mode == 1 )
        {
            /* recovered: &glow->identifier + 54*marker, +92/+96/+100 -> markers[marker].matrix.up */
            object_marker *marker = &glow->markers[particle->parent_marker_index];
            particle->initial_velocity.n[0] = marker->matrix.up.n[0];
            particle->initial_velocity.n[1] = marker->matrix.up.n[1];
            particle->initial_velocity.n[2] = marker->matrix.up.n[2];
        }
        else if ( velocity_mode < 3 )
        {
            unsigned int *seed_x = get_global_local_random_seed_address();
            particle->initial_velocity.n[0] = real_seed_random_range(seed_x, -1.0f, 1.0f);
            unsigned int *seed_y = get_global_local_random_seed_address();
            particle->initial_velocity.n[1] = real_seed_random_range(seed_y, -1.0f, 1.0f);
            unsigned int *seed_z = get_global_local_random_seed_address();
            particle->initial_velocity.n[2] = real_seed_random_range(seed_z, -1.0f, 1.0f);
            normalize3d(&particle->initial_velocity);
        }
    }
    else
    {
        particle->initial_velocity.n[0] = 0.0f;
        particle->initial_velocity.n[1] = 0.0f;
        particle->initial_velocity.n[2] = 1.0f;
    }

    float velocity_scale = definition->trailing_particle_velocity * SECONDS_PER_TICK;
    particle->initial_velocity.n[0] *= velocity_scale;
    particle->initial_velocity.n[1] *= velocity_scale;
    particle->initial_velocity.n[2] *= velocity_scale;

    unsigned int *size_seed = get_global_local_random_seed_address();
    float size = real_seed_random_range(size_seed, definition->particle_size_lower_bound, definition->particle_size_upper_bound);
    particle->initial_size = size / (float)glow->bitmap_dimension;

    particle->lifetime = (int)(definition->trailing_particle_lifetime * 30.0f);

    unsigned int *color_seed = get_global_local_random_seed_address();
    float color_t = real_seed_random_range(color_seed, 0.0f, 1.0f);
    particle->color.n[0] = 1.0f;
    particle->color.n[1] = (definition->color_bound_1.n[1] - definition->color_bound_0.n[1]) * color_t
            + definition->color_bound_0.n[1];
    particle->color.n[2] = (definition->color_bound_1.n[2] - definition->color_bound_0.n[2]) * color_t
            + definition->color_bound_0.n[2];
    particle->color.n[3] = (definition->color_bound_1.n[3] - definition->color_bound_0.n[3]) * color_t
            + definition->color_bound_0.n[3];

    particle->flags |= (1u << _glow_particle_is_trailing_particle_bit);
    return particle;
}
