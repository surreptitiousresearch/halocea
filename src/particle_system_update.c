/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* particle_system_update @0x8373A728 — advance one particle system for dtime. Refreshes the system's
 * object-driven scale/position/velocity, runs its system-physics update, then for each emitter type
 * advances its state machine (interpolating emitter variables across state transitions), emits new
 * particles, and steps every live particle's own state machine + sprite/rotation animation, reaping
 * dead particles. A system with no surviving particles and no host object is deleted.
 *
 * Particle system definition tag (v8, raw offsets): system update style @72, type count @92, type block
 * @96 (128-byte particle_system_type records). Type state record (192 bytes): randomized-multiplier
 * source block @48, state-length random range floats @32/@36, particle update function index @178.
 * Particle state record (376 bytes): state-length random range floats @32/@36, time range floats[8]/[9]
 * (@32/@36), transition ranges @40/@44. Object flags bit 11 (_object_connected_to_map_bit) gates the
 * function-driven scale update. type_definition->flags is particle_system_type_flags (see header). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/object_flags.h"
#include "headers/particle_system_definition.h"
#include "headers/particle_system_type_state.h"
#include "headers/particle_system_type_particle_state.h"
#include "headers/global_tag_instances.h"
#include "headers/particle_system_datum.h"
#include "headers/particle_system_flags.h"
#include "headers/particle_system_type_flags.h"
#include "headers/particle_system_type.h"
#include "headers/ps_particle_datum.h"
#include "headers/particle_type.h"
#include "headers/real_point3d.h"
#include "headers/blam_data_globals.h"
#include "headers/real_vector3d.h"
#include "headers/particle_system_type_particle_state_interpolated_randomized_variables.h"
extern void *memcpy(void *dst, const void *src, unsigned int size);


extern void (*system_update_functions[])(particle_system_datum *system, double dtime);
extern void (*particle_update_functions[])(particle_system_datum *system, int type_index, double dtime);

extern uint8_t object_get_function_value(int object_index, int16_t function_index, float *value_reference);
extern real_point3d *object_get_origin(int object_index, real_point3d *origin);
extern void object_get_velocities(int object_index, real_vector3d *translational_velocity, real_vector3d *angular_velocity);
extern uint32_t *get_global_local_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);
extern void particle_system_next_type_state_index(const particle_system_datum *system, particle_type *type, const particle_system_type *type_definition);
extern void particle_system_new_particles(particle_system_datum *system, int16_t type_index, float dt);
extern void randomize_particle_variables(particle_system_type *type_definition, particle_system_type_particle_state_interpolated_randomized_variables *variables, int16_t state_index);
extern void particle_system_next_particle_state_index(ps_particle_datum *particle, const particle_system_type *type_definition);
extern void particle_system_delete(int system_index);
extern void datum_delete(data_array *data, int index);

void particle_system_update(float dtime, int particle_system_index)
{
    particle_system_datum *system = DATA_ARRAY_ELEMENT(particle_systems, particle_system_datum, particle_system_index);
    int16_t live_type_count = 0;
    int object_index = system->object_index;
    /* DEVIATION: decompiler typed this as int; it is dereferenced as a pointer below, so it is the
     * loaded particle_system_definition tag pointer. */
    particle_system_definition *definition = TAG_GET(particle_system_definition, system->definition_index);
    int type_index;

    if ( object_index != -1 )
    {
        object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, system->object_index)->datum;
        /* DEVIATION: prior source read *(datum)+16 (definition_index+16); decompiler reads (datum ptr)+16 =
         * object.flags @0x10. Typed container restores that. 0x800 = bit 11 = connected-to-map. */
        if ( (object->object.flags & (1u << _object_connected_to_map_bit)) != 0
          && object_get_function_value(object_index, system->function_index, &system->scale) )
            system->flags |= (1u << _particle_system_active_bit);
        else
            system->flags &= ~(1u << _particle_system_active_bit);

        object_get_origin(system->object_index, &system->position);
        object_get_velocities(system->object_index, &system->velocity, NULL);
        system->velocity.n[0] = system->velocity.n[0] * 30.0f;
        system->velocity.n[1] = system->velocity.n[1] * 30.0f;
        system->velocity.n[2] = system->velocity.n[2] * 30.0f;
    }

    system_update_functions[definition->system_update_physics](system, dtime);

    for ( type_index = 0; type_index < definition->types.count; )
    {
        /* recovered: (type_index << 7) + base -> typed array index (stride 128 == sizeof(particle_system_type)) */
        particle_system_type *type_definition = &((particle_system_type *)definition->types.address)[type_index];
        particle_type *type = &system->types[type_index];

        if ( (type_definition->flags & (1u << _particle_system_type_disabled_bit)) == 0 )
        {
            type->time_left_in_state = type->time_left_in_state - dtime;

            if ( (uint16_t)type->state_index != 0xFFFF )
            {
                particle_system_type_state *current_state = NULL;
                int16_t transition_state_index;

                /* advance the type state machine until time remains in the current state */
                while ( 1 )
                {
                    current_state = &((particle_system_type_state *)type_definition->type_states.address)[type->state_index];
                    transition_state_index = (uint16_t)type->transition_state_index;
                    if ( type->time_left_in_state >= 0.0 )
                        break;

                    {
                        float lower, upper, span;
                        if ( transition_state_index == 0xFFFF )
                        {
                            particle_system_next_type_state_index(system, type, type_definition);
                            upper = current_state->transition_time_upper_bound;
                            lower = current_state->transition_time_lower_bound;
                        }
                        else
                        {
                            particle_system_type_state *transition_state =
                                &((particle_system_type_state *)type_definition->type_states.address)[transition_state_index];
                            type->state_index = transition_state_index;
                            type->transition_state_index = -1;
                            upper = transition_state->duration_upper_bound;
                            lower = transition_state->duration_lower_bound;
                        }
                        span = real_seed_random_range(get_global_local_random_seed_address(), lower, upper);
                        type->time_left_in_state = type->time_left_in_state + span;
                        type->state_length = span;
                    }
                    if ( (uint16_t)type->state_index == 0xFFFF )
                        goto type_state_done;
                }

                /* refresh interpolated emitter variables for the current/transition state */
                if ( transition_state_index == -1 )
                {
                    memcpy(&type->variables, &current_state->variables, 40);   /* 10 dwords */
                }
                else
                {
                    particle_system_type_state *transition_state =
                        &((particle_system_type_state *)type_definition->type_states.address)[transition_state_index];
                    float t = (type->time_left_in_state / type->state_length);
                    float *dest = (float *)&type->variables;
                    const float *current_vars = (const float *)&current_state->variables;
                    const float *transition_vars = (const float *)&transition_state->variables;
                    int k;

                    if ( t < 0.0 )
                        t = 0.0f;
                    else if ( t > 1.0 )
                        t = 1.0f;

                    for ( k = 0; k < 10; ++k )
                        dest[k] = (current_vars[k] * t) + (transition_vars[k] * (1.0f - t));
                }

                /* apply the system-level color/scale modulation flags */
                if ( (type_definition->flags & (1u << _particle_system_type_tint_by_effect_color_bit)) != 0 )
                {
                    type->variables.particle_state_randomized_multipliers.color.n[0] = system->color.n[0] * type->variables.particle_state_randomized_multipliers.color.n[0];
                    type->variables.particle_state_randomized_multipliers.color.n[1] = system->color.n[1] * type->variables.particle_state_randomized_multipliers.color.n[1];
                    type->variables.particle_state_randomized_multipliers.color.n[2] = system->color.n[2] * type->variables.particle_state_randomized_multipliers.color.n[2];
                    type->variables.particle_state_randomized_multipliers.color.n[3] = system->color.n[3] * type->variables.particle_state_randomized_multipliers.color.n[3];
                }
                if ( (type_definition->flags & (1u << _particle_system_type_minimum_count_scales_bit)) != 0 )
                    type->variables.minimum_particle_count = system->scale * type->variables.minimum_particle_count;
                if ( (type_definition->flags & (1u << _particle_system_type_creation_rate_scales_bit)) != 0 )
                    type->variables.particle_creation_rate = system->scale * type->variables.particle_creation_rate;
                if ( (type_definition->flags & (1u << _particle_system_type_scale_scales_bit)) != 0 )
                    type->variables.particle_state_randomized_multipliers.scale = system->scale * type->variables.particle_state_randomized_multipliers.scale;
                if ( (type_definition->flags & (1u << _particle_system_type_animation_rate_scales_bit)) != 0 )
                    type->variables.particle_state_randomized_multipliers.animation_rate = system->scale * type->variables.particle_state_randomized_multipliers.animation_rate;
                if ( (type_definition->flags & (1u << _particle_system_type_rotation_rate_scales_bit)) != 0 )
                    type->variables.particle_state_randomized_multipliers.rotation_rate = system->scale * type->variables.particle_state_randomized_multipliers.rotation_rate;
            }

type_state_done:
            if ( (uint16_t)type->state_index != 0xFFFF )
            {
                ps_particle_datum *previous = NULL;
                int particle_index;

                if ( (system->flags & (1u << _particle_system_active_bit)) != 0 )
                    particle_system_new_particles(system, type_index, dtime);

                particle_index = (int16_t)type->first_particle_index;
                if ( particle_index != -1 )
                {
                    int current_index = particle_index;
                    do
                    {
                        /* recovered: (char*)data + ((particle_index<<7)&0x7FFF80) -> typed element at datum absolute index (stride 128 == sizeof(ps_particle_datum), unchecked) */
                        ps_particle_datum *particle = DATA_ARRAY_ELEMENT(system_particles, ps_particle_datum, particle_index);

                        particle->time_left_in_state = particle->time_left_in_state - dtime;
                        if ( (uint16_t)particle->state_index == 0xFFFF && type_definition->particle_states.count > 0 )
                        {
                            particle_system_type_particle_state *first_state = (particle_system_type_particle_state *)type_definition->particle_states.address;
                            float span = real_seed_random_range(get_global_local_random_seed_address(),
                                                               first_state->duration_lower_bound, first_state->duration_upper_bound);
                            particle->state_index = 0;
                            particle->time_left_in_state = span;
                            particle->state_length = span;
                            randomize_particle_variables(type_definition, &particle->randomized_variables, particle->state_index);
                        }
                        if ( !particle->valid )
                            particle->state_index = -1;

                        /* advance the per-particle state machine */
                        while ( (uint16_t)particle->state_index != 0xFFFF )
                        {
                            particle_system_type_particle_state *particle_state = &((particle_system_type_particle_state *)type_definition->particle_states.address)[particle->state_index];
                            int16_t particle_transition;
                            float lower, upper, span;

                            if ( particle->time_left_in_state >= 0.0 )
                                break;

                            if ( (uint16_t)particle->transition_state_index == 0xFFFF )
                            {
                                particle_system_next_particle_state_index(particle, type_definition);
                                upper = particle_state->transition_time_upper_bound;
                                lower = particle_state->transition_time_lower_bound;
                            }
                            else
                            {
                                int16_t next = particle->transition_state_index;
                                particle_system_type_particle_state *next_state;
                                particle->state_index = next;
                                particle->transition_state_index = -1;
                                next_state = &((particle_system_type_particle_state *)type_definition->particle_states.address)[particle->state_index];
                                upper = next_state->duration_upper_bound;
                                lower = next_state->duration_lower_bound;
                            }
                            particle_transition = (uint16_t)particle->transition_state_index;
                            span = real_seed_random_range(get_global_local_random_seed_address(), lower, upper);
                            particle->time_left_in_state = span + particle->time_left_in_state;
                            particle->state_length = span;
                            if ( particle_transition == 0xFFFF )
                                memcpy(&particle->randomized_variables, &particle->transition_randomized_variables, 28); /* 7 dwords */
                            else
                                randomize_particle_variables(type_definition, &particle->transition_randomized_variables, particle_transition);
                        }

                        if ( (uint16_t)particle->state_index == 0xFFFF )
                        {
                            /* particle died: unlink and free */
                            int next = particle->next_particle_index;
                            if ( previous )
                                previous->next_particle_index = next;
                            else
                                type->first_particle_index = next;
                            datum_delete(system_particles, current_index);
                            particle_index = (int16_t)particle->next_particle_index;
                            --type->particle_count;
                        }
                        else
                        {
                            /* animate sprite + rotation, then run the per-state physics update */
                            particle_system_type_state *type_state = &((particle_system_type_state *)type_definition->type_states.address)[type->state_index];
                            if ( (uint16_t)particle->transition_state_index == 0xFFFF )
                            {
                                particle->rotation = ((particle->randomized_variables.rotation_rate
                                                * type->variables.particle_state_randomized_multipliers.rotation_rate) * dtime) + particle->rotation;
                                particle->sprite_index = ((particle->randomized_variables.animation_rate
                                                * type->variables.particle_state_randomized_multipliers.animation_rate) * dtime) + particle->sprite_index;
                            }
                            else
                            {
                                float t = (particle->time_left_in_state / particle->state_length);
                                float animation_rate;
                                if ( t < 0.0 )
                                    t = 0.0f;
                                else if ( t > 1.0 )
                                    t = 1.0f;
                                animation_rate = (particle->transition_randomized_variables.animation_rate * (1.0f - t))
                                               + (particle->randomized_variables.animation_rate * t);
                                particle->rotation = ((((particle->transition_randomized_variables.rotation_rate * (1.0f - t))
                                                + (particle->randomized_variables.rotation_rate * t))
                                                * type->variables.particle_state_randomized_multipliers.rotation_rate) * dtime) + particle->rotation;
                                particle->sprite_index = ((animation_rate
                                                * type->variables.particle_state_randomized_multipliers.animation_rate) * dtime) + particle->sprite_index;
                            }
                            particle_update_functions[type_state->particle_update_physics](system, type_index, dtime);
                            previous = particle;
                            particle_index = (int16_t)particle->next_particle_index;
                        }
                        current_index = (int16_t)particle_index;
                    }
                    while ( (int16_t)particle_index != -1 );
                }
                ++live_type_count;
            }
        }

        type_index = (int16_t)(type_index + 1);
    }

    system->flags &= ~(1u << _particle_system_initializing_bit);
    if ( !live_type_count && system->object_index == -1 )
        particle_system_delete(particle_system_index);
}
