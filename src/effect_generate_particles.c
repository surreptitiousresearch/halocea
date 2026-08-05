/* effect_generate_particles @0x836E2B40 — emit the continuous particles for the effect's current event
 * this frame. For each particle definition in the event, the number to spawn is the difference between
 * the count-function integral evaluated at this frame's event fraction and at the previous frame's, times
 * the part's randomized particle count. Each particle gets a randomized emission position (within a
 * sphere offset along the marker direction), translational velocity, radius, angular velocity, rotation
 * and color, transformed into world space through the marker's matrix or its object / first-person node.
 *
 * Particle record typed (effect_particles_definition, 232 bytes): environment @0 (word), disposition @2 (word), location
 * filter / camera mode @4 (word), location index @8 (word), emission offset vector @20, velocity forward
 * @32, particle tag @96, flags @100 (0x1 attach, 0x2 random rotation, 0x4 modulate by effect color,
 * bits3-4 color interp function), count function @104 (word), emission radius range @112/@116,
 * translational velocity range @132/@136 + cone @140, angular velocity range @144/@148, radius range
 * @160/@164, color alpha/rgb lower @176/@180, upper @192/@196, a-scale mask @224, b-scale mask @228. */

#include <stdint.h>
#include "headers/global_tag_instances.h"
#include "headers/effect_definition.h"
#include "headers/effect_flags.h"
#include "headers/effect_event_definition.h"
#include "headers/effect_particles_definition.h"
#include "headers/effect_particle_definition_flags.h"
#include "headers/effect_particle_flags.h"
#include "headers/effect_velocity_scaleable_values_flags.h"
#include "headers/effect_disposition.h"
#include "headers/effect_camera_mode.h"
#include "headers/effect_datum.h"
#include "headers/effect_node_designator.h"
#include "headers/effect_location_datum.h"
#include "headers/new_particle_data.h"
#include "headers/real_matrix4x3.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_rgb_color.h"
#include "headers/math_constants.h"
#include "headers/blam_data_globals.h"


extern float effect_evaluate_function_integral(int16_t function, float fraction);
extern effect_location_datum *effect_location_get_next_instance(const effect_datum *effect, int *location_datum_index, int16_t camera_mode);
extern int first_person_weapon_get_weapon_index(int16_t local_player_index);
extern real_matrix4x3 *first_person_weapon_get_node_matrix(int16_t local_player_index, int16_t node_index);
extern real_matrix4x3 *object_get_node_matrix(int object_index, int16_t node_index);
extern uint32_t *get_global_local_random_seed_address(void);
extern float real_seed_random(uint32_t *seed);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);
extern real_vector3d *seed_random_direction3d(uint32_t *seed, real_vector3d *direction);
extern real_point3d *matrix4x3_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);
extern real_vector3d *matrix4x3_transform_normal(const real_matrix4x3 *matrix, const real_vector3d *normal, real_vector3d *result);
extern real_vector3d *matrix4x3_transform_vector(const real_matrix4x3 *matrix, const real_vector3d *vector, real_vector3d *result);
extern void effect_random_translational_velocity(uint32_t *seed, const effect_datum *effect, const real_vector3d *forward, real_vector3d *direction, real_vector3d *velocity, float lower_bound, float upper_bound, float cone_angle, unsigned int a_scale_flags, unsigned int b_scale_flags);
extern uint8_t effect_allowed_by_environment(int16_t environment, const location *location, const real_point3d *world_point);
extern real_rgb_color *rgb_colors_interpolate(real_rgb_color *rgb_result, unsigned int flags, const real_rgb_color *rgb_lower_bound, const real_rgb_color *rgb_upper_bound, float u);
extern void particle_new(const new_particle_data *data);

/* scaled random range helper matching the inline (scale_a bit / scale_b bit) pattern used throughout */
static float effect_scaled_random_range(const effect_datum *effect, float lower, float upper,
        unsigned int a_scales, unsigned int b_scales, unsigned int bit)
{
    float base = lower;
    float span = upper - lower;
    if ( (a_scales & bit) != 0 )
        base = effect->scale_a * lower;
    if ( (b_scales & bit) != 0 )
        base = effect->scale_b * base;
    if ( (a_scales & (bit << 1)) != 0 )
        span = effect->scale_a * (upper - lower);
    if ( (b_scales & (bit << 1)) != 0 )
        span = effect->scale_b * span;
    return real_seed_random_range(get_global_local_random_seed_address(), 0.0f, span) + base;
}

extern float effect_scale(const effect_datum *effect, float value, unsigned int scale_a_flags, unsigned int scale_b_flags, char bit_index);
void effect_generate_particles(effect_datum *effect)
{
    float last_event_fraction;
    effect_definition *definition;
    effect_event_definition *event_record;
    float fraction;
    int part_index;

    if ( !render_particles_enabled )
        return;

    last_event_fraction = effect->last_event_fraction;
    definition = TAG_GET(effect_definition, effect->definition_index);
    event_record = &((effect_event_definition *)definition->events.address)[effect->event_index];
    fraction = effect->event_duration <= 0.0 ? 1.0f : (effect->event_time / effect->event_duration);

    if ( event_record->particles.count <= 0 )
        goto done;

    for ( part_index = 0; part_index < event_record->particles.count; part_index = (int16_t)(part_index + 1) )
    {
        effect_particles_definition *part =
            &((effect_particles_definition *)event_record->particles.address)[part_index];
        int location_index = part->location_index;
        int disposition;
        float count_float;
        int16_t count_function;
        int16_t emit_count; /* was int: binary keeps the count 16-bit — lhz of the fctiwz low half @0x836E2C98/0x836E2CAC/0x836E2CD8, extsh at every read (decompiler: __int16 v19/r21) */
        int16_t camera_mode;
        int location_cursor;
        effect_location_datum *instance;
        int remaining;

        if ( location_index < 0 || location_index >= definition->locations.count )
            continue;
        disposition = part->disposition;
        if ( (((effect->flags >> _effect_nonviolent_bit) & 1) != 0
                  ? disposition - _effect_disposition_violent
                  : disposition - _effect_disposition_nonviolent) == 0 )
            continue;

        count_function = part->distribution_function;
        count_float = (float)effect->particles_counts[part_index];
        emit_count = (int)(effect_evaluate_function_integral(count_function, fraction) * count_float)
                   - (int)(effect_evaluate_function_integral(count_function, last_event_fraction) * count_float);
        if ( render_particles_enabled == 1 )
            emit_count = (int)((float)emit_count * 0.5f);
        if ( emit_count <= 0 )
            continue;

        camera_mode = part->camera_mode;
        location_cursor = effect->location_datum_indices[location_index];
        instance = effect_location_get_next_instance(effect, &location_cursor, camera_mode);

        while ( instance )
        {
            uint16_t node_designator = (uint16_t)instance->node_designator;

            /* skip first-person emission locations when there is no first-person weapon shown */
            if ( node_designator == 0xFFFF || !EFFECT_NODE_IS_FIRST_PERSON_WEAPON(node_designator)
              || first_person_weapon_get_weapon_index(effect->local_player_index) != -1 )
            {
                break;
            }
            instance = effect_location_get_next_instance(effect, &location_cursor, camera_mode);
        }
        if ( !instance )
            continue;

        remaining = emit_count;
        while ( 1 )
        {
            unsigned int a_scales = part->a_scales;
            unsigned int b_scales = part->b_scales;
            float emission_radius;
            real_vector3d random_direction;
            real_vector3d emission_offset;
            new_particle_data particle;
            real_point3d position;
            real_vector3d direction;
            real_vector3d velocity;
            uint16_t node_designator;
            float color_fraction;

            --remaining;

            /* emission position: marker origin + random unit sphere * radius + transformed offset */
            emission_radius = effect_scaled_random_range(effect, part->distribution_radius_lower_bound,
                                  part->distribution_radius_upper_bound, a_scales, b_scales,
                                  1u << _effect_particle_distribution_radius_bit);
            seed_random_direction3d(get_global_local_random_seed_address(), &random_direction);
            matrix4x3_transform_vector(&instance->matrix, &part->offset, &emission_offset);
            particle.position.n[0] = ((random_direction.n[0] * emission_radius) + instance->matrix.n[3][0]) + emission_offset.n[0];
            particle.position.n[1] = ((random_direction.n[1] * emission_radius) + emission_offset.n[1]) + instance->matrix.n[3][1];
            particle.position.n[2] = ((random_direction.n[2] * emission_radius) + instance->matrix.n[3][2]) + emission_offset.n[2];

            effect_random_translational_velocity(get_global_local_random_seed_address(), effect,
                    &part->runtime_direction, &particle.direction, &particle.velocity,
                    part->velocity_lower_bound, part->velocity_upper_bound, part->velocity_cone_angle, a_scales, b_scales);
            matrix4x3_transform_normal(&instance->matrix, &particle.direction, &particle.direction);
            matrix4x3_transform_vector(&instance->matrix, &particle.velocity, &particle.velocity);

            /* transform position/direction/velocity into world space through the node, if any */
            node_designator = (uint16_t)instance->node_designator;
            if ( node_designator == 0xFFFF )
            {
                position = particle.position;
                direction = particle.direction;
                velocity = particle.velocity;
            }
            else
            {
                const real_matrix4x3 *node_matrix;
                if ( EFFECT_NODE_IS_FIRST_PERSON_WEAPON(node_designator) )
                    node_matrix = first_person_weapon_get_node_matrix(effect->local_player_index, EFFECT_NODE_DESIGNATOR_TO_INDEX(node_designator));
                else
                    node_matrix = object_get_node_matrix(effect->object_index,
                                      ((int16_t)node_designator == -1) ? -1 : EFFECT_NODE_DESIGNATOR_TO_INDEX(node_designator));
                matrix4x3_transform_point(node_matrix, &particle.position, &position);
                matrix4x3_transform_normal(node_matrix, &particle.direction, &direction);
                matrix4x3_transform_vector(node_matrix, &particle.velocity, &velocity);
            }

            if ( effect_allowed_by_environment(part->environment, &effect->location, &position) )
            {
                int flags = part->flags;
                particle.definition_index = part->particle.index;

                if ( (flags & (1u << _effect_particle_attached_bit)) != 0 )
                {
                    /* attached particle: rides its object node, no inherited velocity */
                    int node = instance->node_designator;
                    particle.object_index = effect->object_index;
                    particle.node_index = (node != -1) ? EFFECT_NODE_DESIGNATOR_TO_INDEX(node) : -1;
                    particle.initial_impulse.n[0] = global_zero_vector3d->n[0];
                    particle.initial_impulse.n[1] = global_zero_vector3d->n[1];
                    particle.initial_impulse.n[2] = global_zero_vector3d->n[2];
                }
                else
                {
                    void (__fastcall *translational)(real_vector3d *, const real_point3d *, void *) = effect->impulse_field.translational;
                    if ( translational )
                        translational(&particle.initial_impulse, &position, effect->impulse_field.identifier);
                    else
                    {
                        particle.initial_impulse.n[0] = global_zero_vector3d->n[0];
                        particle.initial_impulse.n[1] = global_zero_vector3d->n[1];
                        particle.initial_impulse.n[2] = global_zero_vector3d->n[2];
                    }
                    particle.velocity.n[0] = (effect->velocity.n[0] * 30.0f) + velocity.n[0];
                    particle.velocity.n[1] = (effect->velocity.n[1] * 30.0f) + velocity.n[1];
                    particle.velocity.n[2] = (effect->velocity.n[2] * 30.0f) + velocity.n[2];
                    particle.object_index = -1;
                    particle.position = position;
                    particle.direction = direction;
                }

                particle.radius = effect_scaled_random_range(effect, part->radius_lower_bound, part->radius_upper_bound,
                                      a_scales, b_scales, 1u << _effect_particle_radius_bit);
                particle.angular_velocity = effect_scaled_random_range(effect,
                                      part->angular_velocity_lower_bound, part->angular_velocity_upper_bound,
                                      a_scales, b_scales, 1u << _effect_angular_velocity_bit);
                particle.rotation = (flags & (1u << _effect_particle_random_orientation_bit)) != 0
                    ? real_seed_random_range(get_global_local_random_seed_address(), 0.0f, TWO_PI)
                    : 0.0f;

                /* color interpolation factor: scaled constant, or random */
                if ( (a_scales & (1u << _effect_particle_tint_bit)) != 0 || (b_scales & (1u << _effect_particle_tint_bit)) != 0 )
                {
                    /* DEVIATION: collapsed to effect_scale@0x836E1210 (donor is zero-xref, inlined at this site); value folds from literal 1.0f, bit_index folds from _effect_particle_tint_bit (=11). Do NOT touch the file's other 3 scale_a/scale_b blocks (radius/angular_velocity/distribution_radius) -- those are inlined copies of the sibling effect_real_random_range@0x836E1278, a different function. */
                    color_fraction = effect_scale(effect, 1.0f, a_scales, b_scales, _effect_particle_tint_bit);
                }
                else
                {
                    color_fraction = real_seed_random(get_global_local_random_seed_address());
                }

                rgb_colors_interpolate(&particle.color.__s1.rgb, (flags >> _effect_particle_tint_interpolate_hsv_bit) & 3,
                        (const real_rgb_color *)&part->tint_lower_bound.n[1],
                        (const real_rgb_color *)&part->tint_upper_bound.n[1], color_fraction);
                particle.color.n[0] = (((float)1.0 - color_fraction) * part->tint_lower_bound.n[0])
                                    + (part->tint_upper_bound.n[0] * color_fraction);
                if ( (flags & (1u << _effect_particle_tint_from_change_color_bit)) != 0 )
                {
                    /* modulate rgb by the effect's color */
                    float g = (effect->color.n[1] * particle.color.n[2]);
                    float b = (effect->color.n[2] * particle.color.n[3]);
                    particle.color.n[1] = effect->color.n[0] * particle.color.n[1];
                    particle.color.n[2] = g;
                    particle.color.n[3] = b;
                }

                {
                    uint16_t nd = (uint16_t)instance->node_designator;
                    int16_t location_filter = part->camera_mode;
                    particle.local_player_index = effect->local_player_index;
                    particle.attached_to_local_player = (nd != 0xFFFF && EFFECT_NODE_IS_FIRST_PERSON_WEAPON(nd));
                    particle.dont_draw_first_person = (location_filter == _effect_camera_mode_third_person_only);
                    particle.dont_draw_third_person = (location_filter == _effect_camera_mode_first_person_only);
                }
                particle_new(&particle);
            }

            /* the emit count is a single budget across this part's locations: once exhausted, advance to
             * the next marker (without resetting), so the first marker gets the bulk and any remaining
             * markers receive one particle each. */
            if ( remaining <= 0 )
            {
                instance = effect_location_get_next_instance(effect, &location_cursor, part->camera_mode);
                if ( !instance )
                    break;
            }
        }
    }

done:
    effect->last_event_fraction = fraction;
}
