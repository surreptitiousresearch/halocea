/* glow_update @0x8380CA80 — per-tick update for one "glow" effect instance (a trailing light streak
 * attached to an object's markers, e.g. a plasma trail or energy glow). Two entirely different jobs
 * share this entry point:
 *
 *  - First-time setup (glow->initialized == 0 and more than one marker was resolved): builds a marker
 *    visitation order by greedily chasing, from each marker, whichever OTHER marker its own forward axis
 *    points most toward (the highest dot(forward, direction_to_candidate)); accumulates the running
 *    distance walked along that order into marker_time_index[]/total_time (used later to place particles
 *    along the streak); then hands off to glow_particles_initialize and marks the glow initialized.
 *
 *  - Steady-state update (every other call): recomputes the tag-driven rotational/translational velocity
 *    scale factors, advances every "normal" (marker-riding) particle's position/color, then ticks every
 *    "trailing" (released) particle's lifetime/fade/size/velocity/position, retiring expired ones and
 *    spawning new ones at the definition's trailing_particle_generation_frequency.
 *
 * Reconstructed from disassembly (decompiler reported "local variable allocation has failed"). `TAG_INSTANCE`
 * dereferenced once yields the glow_definition* directly (its first field, attachment_marker, is also read
 * as the marker-name argument to object_get_marker_by_name — the definition's own address doubles as that
 * name string). DEVIATION: the decompiler modeled the scratch stack slot backing the per-marker greedy
 * search (`v82`) as a single 8-byte int64, too small for the 5-marker maximum it's actually indexed up to;
 * reproduced here as its own properly-sized array. The `fsel FRT,FRA,FRC,FRB` idiom in the
 * trailing-particle size/velocity decay (`FRT = FRA >= 0.0 ? FRC : FRB`) is a clamp-to-zero-minimum with no
 * upper bound (disasm-confirmed constants: f31 = 1.0, f29 = 0.0, both loaded once and held for the rest of
 * the steady-state branch). */

#include <stdint.h>
#include "headers/glow_datum.h"
#include "headers/glow_particle.h"
#include "headers/glow_globals.h"
#include "headers/glow_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/data_array.h"
#include "headers/render_globals.h"
#include "headers/glow_definition_flags.h"
#include "headers/glow_particle_flags.h"

extern int16_t object_get_marker_by_name(int object_index, const char *name, object_marker *markers, int16_t maximum_marker_count);
extern uint8_t object_get_function_value(int object_index, int16_t function_index, float *value_reference);
extern int16_t game_time_get_elapsed(void);
extern void glow_normal_particle_update_position(int object_index, glow_datum *glow, glow_particle *particle, float elapsed_time, float rotation_per_unit_t);
extern void glow_normal_particle_update_color(int object_index, glow_datum *glow, glow_particle *particle);
extern void glow_particles_initialize(glow_datum *glow);
extern glow_particle * glow_trailing_particle_new(glow_datum *glow);
extern void datum_delete(data_array *data, int index);
extern float __fsqrts(float x);
extern double __fabs(double x);

static float clamp01(float value)
{
    if ( value < 0.0f )
        return 0.0f;
    if ( value > 1.0f )
        return 1.0f;
    return value;
}

void glow_update(glow_datum *glow, int object_index)
{
    glow_definition *definition = TAG_GET(glow_definition, glow->definition_index);
    if ( !definition )
        return;

    int16_t found_marker_count = object_get_marker_by_name(object_index, (const char *)definition, glow->markers, 5);
    glow->number_of_markers = found_marker_count;

    if ( !glow->initialized && found_marker_count > 1 )
    {
        /* first-time setup: build the marker visitation order */
        int16_t candidate_best_index[5];
        for ( int16_t marker_index = 0; marker_index < found_marker_count; ++marker_index )
        {
            int16_t best_index = -1;
            float best_dot = 0.0f;
            for ( int16_t candidate_index = 0; candidate_index < found_marker_count; ++candidate_index )
            {
                if ( marker_index == candidate_index )
                    continue;

                float dx = glow->markers[candidate_index].matrix.position.n[0]
                        - glow->markers[marker_index].matrix.position.n[0];
                float dy = glow->markers[candidate_index].matrix.position.n[1]
                        - glow->markers[marker_index].matrix.position.n[1];
                float dz = glow->markers[candidate_index].matrix.position.n[2]
                        - glow->markers[marker_index].matrix.position.n[2];
                float distance = __fsqrts(dx * dx + (dz * dz + dy * dy));
                if ( __fabs(distance) >= 0.000099999997 )
                {
                    float inv_distance = 1.0f / distance;
                    dx *= inv_distance;
                    dy *= inv_distance;
                    dz *= inv_distance;
                }

                float forward_dot = glow->markers[marker_index].matrix.forward.n[0] * dx
                        + (glow->markers[marker_index].matrix.forward.n[2] * dz
                            + glow->markers[marker_index].matrix.forward.n[1] * dy);
                if ( forward_dot > best_dot )
                {
                    best_index = marker_index;
                    best_dot = forward_dot;
                }
            }
            candidate_best_index[marker_index] = best_index;
        }

        int16_t previous_best = -1;
        for ( int16_t order_slot = (int16_t)(found_marker_count - 1); order_slot >= 0; --order_slot )
        {
            int16_t search_index = (int16_t)(glow->number_of_markers - 1);
            for ( ; search_index >= 0; --search_index )
            {
                if ( candidate_best_index[search_index] == previous_best )
                {
                    glow->marker_order[order_slot] = search_index;
                    break;
                }
            }
            previous_best = search_index;
        }

        glow->marker_time_index[0] = 0.0f;
        glow->total_time = 0.0f;
        for ( int16_t order_index = 0; order_index < glow->number_of_markers - 1; ++order_index )
        {
            object_marker *from_marker = &glow->markers[glow->marker_order[order_index]];
            object_marker *to_marker = &glow->markers[glow->marker_order[order_index + 1]];
            float dx = to_marker->matrix.position.n[0] - from_marker->matrix.position.n[0];
            float dy = to_marker->matrix.position.n[1] - from_marker->matrix.position.n[1];
            float dz = to_marker->matrix.position.n[2] - from_marker->matrix.position.n[2];
            glow->total_time += __fsqrts(dx * dx + (dz * dz + dy * dy));
            glow->marker_time_index[order_index + 1] = glow->total_time;
        }

        glow_particles_initialize(glow);
        glow->initialized = 1;
        glow->accumulated_trailing_particle_generation_ticks = 0;
        return;
    }

    /* steady-state update */
    float effect_rotational_velocity = definition->glow_effect_rotational_velocity;
    float effect_translational_velocity = definition->glow_effect_translational_velocity;
    if ( found_marker_count > 1 )
    {
        if ( definition->effect_rotational_velocity_attachment_index != 0xFFFF )
        {
            float t = 0.0f;
            object_get_function_value(object_index, definition->effect_rotational_velocity_attachment_index, &t);
            effect_rotational_velocity = ((definition->glow_effect_rotational_velocity_scale_upper_bound
                        - definition->glow_effect_rotational_velocity_scale_lower_bound) * t
                    + definition->glow_effect_rotational_velocity_scale_lower_bound)
                    * definition->glow_effect_rotational_velocity;
        }
        if ( definition->effect_translational_velocity_attachment_index != 0xFFFF )
        {
            float t = 0.0f;
            object_get_function_value(object_index, definition->effect_translational_velocity_attachment_index, &t);
            effect_translational_velocity = ((definition->glow_effect_translational_velocity_scale_upper_bound
                        - definition->glow_effect_translational_velocity_scale_lower_bound) * t
                    + definition->glow_effect_translational_velocity_scale_lower_bound)
                    * definition->glow_effect_translational_velocity;
        }
    }
    float rotation_per_unit_t = effect_rotational_velocity / effect_translational_velocity;

    int16_t elapsed = game_time_get_elapsed();
    glow->accumulated_trailing_particle_generation_ticks += elapsed;

    if ( glow->number_of_markers > 1 )
    {
        for ( glow_particle *particle = glow->head_particle; particle; particle = particle->next )
        {
            if ( (particle->flags & (1u << _glow_particle_is_trailing_particle_bit)) == 0 )
            {
                glow_normal_particle_update_position(object_index, glow, particle,
                        render.time_delta_since_frame_sec * effect_translational_velocity, rotation_per_unit_t);
                glow_normal_particle_update_color(object_index, glow, particle);
                particle->present_size = particle->initial_size;
            }
        }
    }

    for ( glow_particle *particle = glow->head_particle; particle; )
    {
        glow_particle *next_particle = particle->next;
        if ( (particle->flags & (1u << _glow_particle_is_trailing_particle_bit)) != 0 )
        {
            particle->ticks_in_existence += game_time_get_elapsed();

            if ( definition->flags & (1u << _glow_trailing_particles_fade_over_time_bit) )
            {
                float fade = 1.0f - (float)particle->ticks_in_existence / (float)particle->lifetime;
                particle->fade = clamp01(fade);
            }
            else
            {
                particle->fade = 1.0f;
            }

            if ( definition->flags & (1u << _glow_trailing_particles_shrink_over_time_bit) )
            {
                float ratio = 1.0f - (float)particle->ticks_in_existence / (float)particle->lifetime;
                if ( ratio < 0.0f )
                    ratio = 0.0f;
                particle->present_size = ratio * particle->initial_size;
            }

            if ( definition->flags & (1u << _glow_trailing_particles_slow_over_time_bit) )
            {
                float ratio = 1.0f - (float)particle->ticks_in_existence / (float)particle->lifetime;
                if ( ratio < 0.0f )
                    ratio = 0.0f;
                particle->present_velocity.n[0] = particle->initial_velocity.n[0] * ratio;
                particle->present_velocity.n[1] = particle->initial_velocity.n[1] * ratio;
                particle->present_velocity.n[2] = particle->initial_velocity.n[2] * ratio;
            }
            else
            {
                particle->present_velocity = particle->initial_velocity;
            }

            particle->position.n[0] += particle->present_velocity.n[0] * render.time_delta_since_frame_sec;
            particle->position.n[1] += particle->present_velocity.n[1] * render.time_delta_since_frame_sec;
            particle->position.n[2] += particle->present_velocity.n[2] * render.time_delta_since_frame_sec;

            if ( particle->ticks_in_existence > particle->lifetime )
            {
                glow_particle *previous = particle->previous;
                if ( previous )
                    previous->next = next_particle;
                else
                    glow->head_particle = next_particle;
                if ( next_particle )
                    next_particle->previous = previous;
                else
                    glow->tail_particle = previous;
                datum_delete(glow_globals.glow_particle_data, particle->index);
                --glow->number_of_particles;
            }
        }
        particle = next_particle;
    }

    if ( definition->trailing_particle_generation_frequency > 0.0099999998f && game_time_get_elapsed() )
    {
        float ticks_per_particle = 30.0f / definition->trailing_particle_generation_frequency;
        if ( ticks_per_particle < 1.0f )
            ticks_per_particle = 1.0f;

        while ( (float)glow->accumulated_trailing_particle_generation_ticks > ticks_per_particle )
        {
            glow_particle *new_particle = glow_trailing_particle_new(glow);
            if ( !new_particle )
                break;

            glow_particle *tail_particle = glow->tail_particle;
            ++glow->number_of_particles;
            if ( tail_particle )
            {
                tail_particle->next = new_particle;
                new_particle->previous = glow->tail_particle;
            }
            else
            {
                glow->head_particle = new_particle;
            }
            glow->tail_particle = new_particle;
            glow->accumulated_trailing_particle_generation_ticks -= (int16_t)ticks_per_particle;
        }
    }
}
