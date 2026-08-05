/* glow_normal_particle_update_position @0x8380C308 — advance one glow particle's lifetime cursor (t) for the
 * frame and recompute its distance-to-object, then recompute its world position. The glow tag definition's
 * loop mode (+34) selects how t wraps: mode 0 ping-pongs (on overshooting [0, total_time] the direction bit
 * — flags bit 0 — flips and t reflects back into range), while mode 1 wraps modularly (t is repeatedly
 * shifted by total_time). The definition's distance function (+128), when assigned, modulates the particle's
 * distance_to_object between two interpolated bounds.
 *
 * The DB prototype types elapsed_time / rotation_per_unit_t as float (the decompiler widened them to double).
 * Glow definition fields resolved to the DB glow_definition struct: boundary_effect @+34,
 * distance_to_object_attachment_index @+128, minimum/maximum_distance_glow_particle_to_object @+132/+136,
 * distance_to_object_scale_lower/upper_bound @+140/+144. */

#include <stdint.h>
#include "headers/glow_datum.h"
#include "headers/glow_particle.h"
#include "headers/glow_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/glow_effect_boundary_type.h"
#include "headers/glow_particle_flags.h"

extern uint8_t object_get_function_value(int object_index, int16_t function_index, float *value_reference);
extern void get_particle_world_position(glow_datum *glow, glow_particle *particle, float rotation_per_unit_t);

void glow_normal_particle_update_position(int object_index, glow_datum *glow, glow_particle *particle,
    float elapsed_time, float rotation_per_unit_t)
{
    glow_definition *definition = TAG_GET(glow_definition, glow->definition_index);

    uint16_t distance_function_index = definition->distance_to_object_attachment_index;
    if ( distance_function_index != 0xFFFF )
    {
        float value;
        if ( !object_get_function_value(object_index, distance_function_index, &value) )
            value = 0.0f;
        particle->distance_to_object =
            ((definition->distance_to_object_scale_upper_bound - definition->distance_to_object_scale_lower_bound) * value + definition->distance_to_object_scale_lower_bound)
                * (definition->maximum_distance_glow_particle_to_object - definition->minimum_distance_glow_particle_to_object)
            + definition->minimum_distance_glow_particle_to_object;
    }

    unsigned int flags = particle->flags;
    int16_t loop_mode = definition->boundary_effect;
    float new_t;
    float reflect_total_time;
    unsigned int reflect_flags;

    if ( (flags & (1u << _glow_particle_traversal_direction_negative_bit)) != 0 )
    {
        new_t = particle->t - elapsed_time;
        particle->t = new_t;
        if ( loop_mode )
        {
            if ( loop_mode == _glow_effect_boundary_type_wrap && new_t < 0.0f )
            {
                do
                    particle->t = glow->total_time + particle->t;
                while ( particle->t < 0.0f );
            }
            goto finished;
        }
        if ( new_t >= 0.0f )
            goto finished;
        do
            particle->t = glow->total_time + particle->t;
        while ( particle->t < 0.0f );
        reflect_total_time = glow->total_time;
        reflect_flags = flags & ~(1u << _glow_particle_traversal_direction_negative_bit);
        goto reflect;
    }

    new_t = elapsed_time + particle->t;
    particle->t = new_t;
    if ( !loop_mode )
    {
        reflect_total_time = glow->total_time;
        if ( new_t <= reflect_total_time )
            goto finished;
        do
        {
            particle->t = particle->t - reflect_total_time;
            reflect_total_time = glow->total_time;
        }
        while ( particle->t > reflect_total_time );
        reflect_flags = flags | (1u << _glow_particle_traversal_direction_negative_bit);
        goto reflect;
    }
    if ( loop_mode == _glow_effect_boundary_type_wrap )
    {
        float total_time = glow->total_time;
        if ( new_t > total_time )
        {
            do
            {
                particle->t = particle->t - total_time;
                total_time = glow->total_time;
            }
            while ( particle->t > total_time );
        }
    }
    goto finished;

reflect:
    {
        float t = particle->t;
        particle->flags = reflect_flags;
        particle->t = reflect_total_time - t;
    }

finished:
    get_particle_world_position(glow, particle, rotation_per_unit_t);
}
