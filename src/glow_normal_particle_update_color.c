/* glow_normal_particle_update_color @0x8380B8A8 — recompute one glow particle's ARGB color and fade for the
 * frame. The glow tag definition supplies a lower/upper color pair interpolated by an object function value
 * (when a color function is assigned) and, when its flag bit 0 is set, an additional per-lifetime (t)
 * modulation scaled by a stored factor. Alpha is always 1. The particle's fade ramps linearly in over the
 * first half-fade-fraction of its normalized lifetime and out over the last, clamped to [0,1].
 *
 * Glow definition fields resolved to the DB glow_definition struct: color_attachment_index @+176,
 * color_bound_0/color_bound_1 @+180/+196, flags @+40, color_rate_of_change @+244, percentage_edge_fade @+248. */

#include <stdint.h>
#include "headers/glow_datum.h"
#include "headers/glow_particle.h"
#include "headers/glow_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/glow_definition_flags.h"

extern uint8_t object_get_function_value(int object_index, int16_t function_index, float *value_reference);

void glow_normal_particle_update_color(int object_index, glow_datum *glow, glow_particle *particle)
{
    glow_definition *definition = TAG_GET(glow_definition, glow->definition_index);

    unsigned __int16 color_function_index = definition->color_attachment_index;
    if ( color_function_index != 0xFFFF )
    {
        float value;
        float function_value = object_get_function_value(object_index, color_function_index, &value) ? value : 0.0f;

        particle->color.n[1] = (definition->color_bound_1.n[1] - definition->color_bound_0.n[1]) * function_value
                             + definition->color_bound_0.n[1];
        particle->color.n[2] = (definition->color_bound_1.n[2] - definition->color_bound_0.n[2]) * function_value
                             + definition->color_bound_0.n[2];
        particle->color.n[3] = (definition->color_bound_1.n[3] - definition->color_bound_0.n[3]) * function_value
                             + definition->color_bound_0.n[3];
        particle->color.n[0] = 1.0f;
    }

    if ( (definition->flags & (1u << _glow_interpolate_color_per_particle_bit)) != 0 )
    {
        float t = particle->t;
        particle->color.n[1] = (definition->color_bound_1.n[1] - definition->color_bound_0.n[1])
                                   * definition->color_rate_of_change * t
                             + definition->color_bound_0.n[1];
        particle->color.n[2] = (definition->color_bound_1.n[2] - definition->color_bound_0.n[2])
                                   * definition->color_rate_of_change * t
                             + definition->color_bound_0.n[2];
        particle->color.n[0] = 1.0f;
        particle->color.n[3] = (definition->color_bound_1.n[3] - definition->color_bound_0.n[3])
                                   * definition->color_rate_of_change * t
                             + definition->color_bound_0.n[3];
    }

    float normalized_lifetime = particle->t / glow->total_time;
    float half_fade_fraction = definition->percentage_edge_fade * 0.5f;
    if ( normalized_lifetime >= half_fade_fraction )
    {
        if ( normalized_lifetime <= 1.0f - half_fade_fraction )
            particle->fade = 1.0f;
        else
            particle->fade = (1.0f - particle->t / glow->total_time) / half_fade_fraction;
    }
    else
    {
        particle->fade = (particle->t / glow->total_time) / half_fade_fraction;
    }

    float fade = particle->fade;
    if ( fade >= 0.0f )
    {
        if ( fade > 1.0f )
            particle->fade = 1.0f;
    }
    else
    {
        particle->fade = 0.0f;
    }
}
