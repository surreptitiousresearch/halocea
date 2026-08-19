/* particle_new @0x8373E808 — spawn one particle from fully-resolved new_particle_data. Resolves the
 * world position (directly, or through the attaching object / first-person node), discards the particle
 * if its BSP location is invalid or not locally visible, then allocates and initializes a particle datum:
 * random animation/flags, lifetime, frame timing, transform/velocity/color, optional initial impulse
 * (scaled by point mass), optional lighting modulation, and the starting animation sequence/frame.
 *
 * Particle definition (v4): flags dword[0] (0x1 random-reverse, 0x4 random-sequence, 0x40 diffuse-light,
 * 0x200 no-vertex-light, 0x400/0x800 random anim bits), point physics tag @32, bitmap tag @16, life span
 * range @56/@60, frame rate range @128/@132, radius animation @116/@120. Bitmap: sequence block @ dword[22]
 * (88), 64-byte sequence records (frame count @52). */

#include <stdint.h>
#include "headers/global_tag_instances.h"
#include "headers/particle_definition.h"
#include "headers/bitmap_group.h"
#include "headers/bitmap_group_sequence.h"
#include "headers/data_array.h"
#include "headers/particle_datum.h"
#include "headers/new_particle_data.h"
#include "headers/point_physics_definition.h"
#include "headers/render_globals.h"
#include "headers/real_matrix4x3.h"
#include "headers/real_point3d.h"
#include "headers/real_rgb_color.h"
#include "headers/location.h"
#include "headers/particle_definition_flags.h"
#include "headers/particle_flags.h"
#include "headers/particle_state.h"
#include "headers/blam_data_globals.h"


#include "headers/data_array.h"
extern real_matrix4x3 *first_person_weapon_get_node_matrix(int16_t local_player_index, int16_t node_index);
extern real_matrix4x3 *object_get_node_matrix(int object_index, int16_t node_index);
extern real_point3d *matrix4x3_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);
extern void scenario_location_from_point(location *location, const real_point3d *point);
extern uint8_t scenario_location_potentially_visible_local(const location *location);
extern int datum_new(data_array *data);
extern uint32_t *get_global_local_random_seed_address(void);
extern uint16_t seed_random(uint32_t *seed);
extern int16_t seed_random_range(uint32_t *seed, int16_t lower_bound, int16_t upper_bound);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);
extern int16_t local_player_count(void);
extern float point_physics_definition_get_mass(const point_physics_definition *physics, float radius);
extern void light_particle(const real_point3d *point, real_rgb_color *light_color, real_rgb_color *diffuse_color, uint8_t block);
extern uint8_t particle_next_sequence(int particle_index);
extern void *datum_get(data_array *data, int index);

void particle_new(const new_particle_data *data)
{
    particle_definition *definition;
    real_point3d world_position;
    location particle_location;
    int index;
    particle_datum *particle;

    if ( data->definition_index == -1 )
        return;

    definition = TAG_GET(particle_definition, data->definition_index);

    if ( data->object_index == -1 )
    {
        world_position = data->position;
    }
    else
    {
        const real_matrix4x3 *node_matrix = data->attached_to_local_player
            ? first_person_weapon_get_node_matrix(data->local_player_index, data->node_index)
            : object_get_node_matrix(data->object_index, data->node_index);
        matrix4x3_transform_point(node_matrix, &data->position, &world_position);
    }

    scenario_location_from_point(&particle_location, &world_position);
    if ( particle_location.leaf_index == -1 || !scenario_location_potentially_visible_local(&particle_location) )
        return;

    index = datum_new(particle_data);
    if ( index == -1 )
        return;

    particle = datum_get(particle_data, index);
    particle->flags = 0;
    if ( (definition->flags & (1u << _particle_can_animate_backwards_bit)) != 0 )
        particle->flags |= seed_random(get_global_local_random_seed_address()) & (1u << _particle_animates_backwards_bit);
    if ( (definition->flags & (1u << _particle_random_u_mirror_bit)) != 0 )
        particle->flags |= seed_random(get_global_local_random_seed_address()) & (1u << _particle_u_mirror_bit);
    if ( (definition->flags & (1u << _particle_random_v_mirror_bit)) != 0 )
        particle->flags |= seed_random(get_global_local_random_seed_address()) & (1u << _particle_v_mirror_bit);
    particle->flags = data->dont_draw_first_person
        ? (particle->flags | (1u << _particle_dont_draw_first_person_bit))
        : (particle->flags & ~(1u << _particle_dont_draw_first_person_bit));
    particle->flags = data->dont_draw_third_person
        ? (particle->flags | (1u << _particle_dont_draw_third_person_bit))
        : (particle->flags & ~(1u << _particle_dont_draw_third_person_bit));
    particle->flags = data->attached_to_local_player
        ? (particle->flags | (1u << _particle_attached_to_local_player_bit))
        : (particle->flags & ~(1u << _particle_attached_to_local_player_bit));

    particle->definition_index = data->definition_index;
    particle->local_player_index = data->local_player_index;
    particle->object_index = data->object_index;
    particle->node_index = data->node_index;
    particle->state = _particle_state_next_sequence_initial;
    particle->last_rendered_frame_index = render.frame_index;

    particle->life_span = real_seed_random_range(get_global_local_random_seed_address(),
                              definition->life_span_lower_bound, definition->life_span_upper_bound);
    if ( particle->life_span > 0.69999999 )
    {
        int players = local_player_count();
        particle->life_span = (((particle->life_span - 0.69999999f)) / (float)players) + 0.69999999f;
    }

    if ( definition->frames_per_second_upper_bound == 0.0 )
        particle->frame_span = 3.4028235e38f;
    else
        particle->frame_span = ((float)1.0 / real_seed_random_range(get_global_local_random_seed_address(),
                                       definition->frames_per_second_lower_bound, definition->frames_per_second_upper_bound));
    particle->frame_time = -1.0f;
    particle->location = particle_location;
    particle->position = data->position;
    particle->direction = data->direction;
    particle->rotation = data->rotation;
    particle->radius = data->radius;
    particle->color.n[0] = data->color.n[0];
    particle->color.n[1] = data->color.n[1];
    particle->color.n[2] = data->color.n[2];
    particle->color.n[3] = data->color.n[3];
    particle->translational_velocity = data->velocity;

    if ( data->object_index == -1 )
    {
        /* free particle: apply the initial impulse scaled by the point's mass */
        particle_definition *particle_def = TAG_GET(particle_definition, particle->definition_index);
        float radius_value = ((((particle->life_time / particle->life_span)
                                  * (particle_def->final_radius_modifier - particle_def->initial_radius_modifier))
                                  + particle_def->initial_radius_modifier) * particle->radius);
        float mass = point_physics_definition_get_mass(
                         TAG_GET(point_physics_definition, definition->physics.index), radius_value);
        particle->translational_velocity.n[0] = (data->initial_impulse.n[0] * mass) + particle->translational_velocity.n[0];
        particle->translational_velocity.n[1] = (data->initial_impulse.n[1] * mass) + particle->translational_velocity.n[1];
        particle->translational_velocity.n[2] = (data->initial_impulse.n[2] * mass) + particle->translational_velocity.n[2];
    }
    particle->angular_velocity = data->angular_velocity;

    if ( (definition->flags & (1u << _particle_self_illuminated_bit)) == 0 || (definition->flags & (1u << _particle_tint_with_diffuse_texture_bit)) != 0 )
    {
        real_rgb_color vertex_light;
        real_rgb_color diffuse_light;
        light_particle(&world_position, &vertex_light, &diffuse_light, 0);
        if ( (definition->flags & (1u << _particle_self_illuminated_bit)) == 0 )
        {
            particle->color.n[1] = particle->color.n[1] * vertex_light.n[0];
            particle->color.n[2] = (particle->color.n[2] * vertex_light.n[1]);
            particle->color.n[3] = (particle->color.n[3] * vertex_light.n[2]);
        }
        if ( (definition->flags & (1u << _particle_tint_with_diffuse_texture_bit)) != 0 )
        {
            particle->color.n[1] = particle->color.n[1] * diffuse_light.n[0];
            particle->color.n[2] = (particle->color.n[2] * diffuse_light.n[1]);
            particle->color.n[3] = (particle->color.n[3] * diffuse_light.n[2]);
        }
    }

    if ( particle_next_sequence(index) )
    {
        bitmap_group_sequence *sequence_record =
            &((bitmap_group_sequence *)(TAG_GET(bitmap_group, definition->bitmap.index))->sequences.address)[particle->sequence_index];
        if ( (definition->flags & (1u << _particle_animation_starts_on_random_frame_bit)) != 0 )
            particle->frame_index = ((particle->flags & (1u << _particle_animates_backwards_bit)) == 0 ? -1 : 1)
                                  + seed_random_range(get_global_local_random_seed_address(), 0, sequence_record->sprites.count);
        else if ( (particle->flags & (1u << _particle_animates_backwards_bit)) != 0 )
            particle->frame_index = sequence_record->sprites.count;
        else
            particle->frame_index = -1;
    }
}
