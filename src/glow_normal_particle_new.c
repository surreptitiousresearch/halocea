/* glow_normal_particle_new @0x8380C4C8 — allocate and randomize one glow trailing particle from a glow_datum
 * and its glow tag definition. Pulls a fresh glow_particle datum (returns null on pool exhaustion), back-links
 * its own index, then, for each field whose definition marker is 0xFFFF ("randomize"), seeds it from the
 * definition's [min,max] range: distance-to-object, initial size (scaled by 1/bitmap_dimension), and — when the
 * "constant color" flag is clear — a colour lerped across the definition's two colour endpoints (alpha forced
 * to 1). Finally the animation phase is set: for animation mode 1, t is the particle's fractional position in
 * the emission (index/total) scaled by total_time; for mode 0, t is uniform over total_time; both also pick a
 * random initial spin angle in [0, 2pi).
 *
 * The glow tag definition fields are resolved to the DB glow_definition struct; glow_particle fields are
 * named. The int->float conversions the decompiler renders via __int64/OVERLAPPED register-puns are plain
 * fcfid/fdivs (disasm-verified: index/total, and random/bitmap_dimension). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/glow_datum.h"
#include "headers/glow_particle.h"
#include "headers/glow_definition.h"
#include "headers/glow_globals.h"
#include "headers/global_tag_instances.h"
#include "headers/glow_definition_flags.h"
#include "headers/glow_particle_distribution_type.h"
#include "headers/blam_data_globals.h"
#include "headers/math_constants.h"


#include "headers/data_array.h"
extern int datum_new(data_array *data);
extern void *datum_get(data_array *data, int index);
extern uint32_t *get_global_local_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);

glow_particle * glow_normal_particle_new(glow_datum *glow, int16_t index, int16_t total)
{
    glow_definition *definition = TAG_GET(glow_definition, glow->definition_index);

    int particle_index = datum_new(glow_globals.glow_particle_data);
    if ( particle_index == -1 )
        return nullptr;

    glow_particle *particle = datum_get(glow_globals.glow_particle_data, particle_index);
    particle->index = particle_index;

    if ( (uint16_t)definition->distance_to_object_attachment_index == 0xFFFF )   /* DEVIATION: int16_t field — uncast, the promoted -1 never matched, so the unattached randomisation was dead; binary zero-extends, lhz r11,0x80(r31) @0x8380C530 + cmplwi cr6,r11,0xFFFF @0x8380C538 */
    {
        unsigned int *seed = get_global_local_random_seed_address();
        particle->distance_to_object = real_seed_random_range(seed, definition->minimum_distance_glow_particle_to_object,
                                                              definition->maximum_distance_glow_particle_to_object);
    }

    if ( (uint16_t)definition->particle_size_attachment_index == 0xFFFF )   /* DEVIATION: same shape — lhz r11,0x9C(r31) @0x8380C55C + cmplwi cr6,r11,0xFFFF @0x8380C560 */
    {
        unsigned int *seed = get_global_local_random_seed_address();
        particle->initial_size = real_seed_random_range(seed, definition->particle_size_lower_bound,
                                                        definition->particle_size_upper_bound) / (float)glow->bitmap_dimension;
    }

    if ( (uint16_t)definition->color_attachment_index == 0xFFFF && (definition->flags & (1u << _glow_interpolate_color_per_particle_bit)) == 0 )   /* DEVIATION: same shape — lhz r10,0xB0(r31) @0x8380C5A8 + cmplwi cr6,r10,0xFFFF @0x8380C5AC */
    {
        unsigned int *seed = get_global_local_random_seed_address();
        float t = real_seed_random_range(seed, 0.0f, 1.0f);
        particle->color.n[0] = 1.0f;
        particle->color.n[1] = (definition->color_bound_1.n[1] - definition->color_bound_0.n[1]) * t
                             + definition->color_bound_0.n[1];
        particle->color.n[2] = (definition->color_bound_1.n[2] - definition->color_bound_0.n[2]) * t
                             + definition->color_bound_0.n[2];
        particle->color.n[3] = (definition->color_bound_1.n[3] - definition->color_bound_0.n[3]) * t
                             + definition->color_bound_0.n[3];
    }

    if ( definition->normal_particle_distribution )
    {
        if ( definition->normal_particle_distribution == _glow_particle_distribution_type_uniform )
        {
            particle->t = ((float)index / (float)total) * glow->total_time;
            unsigned int *seed = get_global_local_random_seed_address();
            particle->initial_angle = real_seed_random_range(seed, 0.0f, TWO_PI);
        }
        return particle;
    }
    else
    {
        unsigned int *seed = get_global_local_random_seed_address();
        particle->t = real_seed_random_range(seed, 0.0f, glow->total_time);
        unsigned int *angle_seed = get_global_local_random_seed_address();
        particle->initial_angle = real_seed_random_range(angle_seed, 0.0f, TWO_PI);
        return particle;
    }
}
