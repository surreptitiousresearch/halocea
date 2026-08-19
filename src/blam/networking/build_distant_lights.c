/* build_distant_lights @0x836F8E88 — synthesize an object's distant-light environment from a sampled lightmap.
 *
 * Given the lightmap color (the baked indirect light), the diffuse surface color, the geometric surface normal
 * and the dominant radiosity (incident-light) direction, it fills a render_lighting with: an ambient term scaled
 * from the lightmap; a primary distant light pointing back along the radiosity direction, colored by the
 * lightmap; a secondary distant light along the surface normal, colored by diffuse*lightmap-brightness; a
 * reflection tint built from clamped diffuse and lightmap terms; a shadow direction biased by the radiosity
 * "accuracy" (its 4th root); and a shadow color. When flags bit 2 is set the colors are brightened and the
 * reflection tint is forced opaque (an alternate, flatter lighting mode).
 *
 * DEVIATION: the decompiler transposed two parameters — what it calls `lighting`/`ambient_color` is really the
 * `lightmap_color` input, and its phantom `a8` is the real `lighting` output (both resolve to r9/r8 once the
 * soft-float ABI's skipped GPR slot for the float accuracy arg is accounted for). It also rendered the
 * radiosity-accuracy exponentiation as a long-double pair; disasm shows powf(radiosity_accuracy, 0.25). Field
 * writes below use the render_lighting layout. */

#include <math.h>
#include "headers/render_lighting.h"
#include "headers/real_rgb_color.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"
#include "headers/distant_lighting_flags.h"


extern float real_rgb_color_brightness(const real_rgb_color *color);
extern void brighten_real_rgb_color(real_rgb_color *color, float fraction);

static float clamp01(float x)
{
    if ( x < 0.0f )
        return 0.0f;
    if ( x > 1.0f )
        return 1.0f;
    return x;
}

static float clamp_lo(float x, float lo)
{
    if ( x < lo )
        return lo;
    if ( x > 1.0f )
        return 1.0f;
    return x;
}

void build_distant_lights(int flags, const real_vector3d *surface_normal, const real_rgb_color *diffuse_color,
                          const real_vector3d *radiosity_normal, float radiosity_accuracy,
                          const real_rgb_color *lightmap_color, render_lighting *lighting)
{
    float brightness = real_rgb_color_brightness(lightmap_color);

    lighting->ambient_color.n[0] = (lightmap_color->n[0] * object_light_ambient_scale) + object_light_ambient_base;
    lighting->ambient_color.n[1] = (lightmap_color->n[1] * object_light_ambient_scale) + object_light_ambient_base;
    lighting->ambient_color.n[2] = (object_light_ambient_scale * lightmap_color->n[2]) + object_light_ambient_base;
    lighting->distant_light_count = 2;

    /* primary distant light: lightmap-colored, pointing back along the radiosity direction */
    lighting->distant_lights[0].color = *lightmap_color;
    lighting->distant_lights[0].direction.n[0] = -radiosity_normal->n[0];
    lighting->distant_lights[0].direction.n[1] = -radiosity_normal->n[1];
    lighting->distant_lights[0].direction.n[2] = -radiosity_normal->n[2];

    /* secondary distant light: diffuse-colored, along the surface normal */
    lighting->distant_lights[1].color.n[0] = (diffuse_color->n[0] * object_light_secondary_scale) * brightness;
    lighting->distant_lights[1].color.n[1] = (object_light_secondary_scale * brightness) * diffuse_color->n[1];
    lighting->distant_lights[1].color.n[2] = (diffuse_color->n[2] * object_light_secondary_scale) * brightness;
    lighting->distant_lights[1].direction = *surface_normal;

    /* reflection tint: alpha from lightmap brightness, rgb from clamped diffuse*lightmap terms */
    lighting->reflection_tint_color.n[0] = clamp01((brightness * 1.5f) + 0.25f);
    float diffuse_tint_r = clamp01((diffuse_color->n[0] * 3.0f) + 0.5f);
    float diffuse_tint_g = clamp01((diffuse_color->n[1] * 3.0f) + 0.5f);
    float diffuse_tint_b = clamp01((diffuse_color->n[2] * 3.0f) + 0.5f);
    lighting->reflection_tint_color.n[1] = clamp01((lightmap_color->n[0] * 2.0f) + 0.25f) * diffuse_tint_r;
    lighting->reflection_tint_color.n[2] = clamp01((lightmap_color->n[1] * 2.0f) + 0.25f) * diffuse_tint_g;
    lighting->reflection_tint_color.n[3] = clamp01((lightmap_color->n[2] * 2.0f) + 0.25f) * diffuse_tint_b;

    /* shadow direction: the primary light direction, scaled by the radiosity accuracy's 4th root and tilted
     * down so its horizontal length never exceeds sqrt(1/2) */
    float accuracy_factor = powf(radiosity_accuracy, 0.25f);
    float shadow_x = lighting->distant_lights[0].direction.n[0] * accuracy_factor;
    float shadow_y = lighting->distant_lights[0].direction.n[1] * accuracy_factor;
    lighting->shadow_vector.n[0] = shadow_x;
    lighting->shadow_vector.n[1] = shadow_y;
    float horizontal = sqrtf((shadow_x * shadow_x) + (shadow_y * shadow_y));
    if ( horizontal >= 0.70700002f )
    {
        lighting->shadow_vector.n[2] = -0.70700002f;
        lighting->shadow_vector.n[0] = shadow_x * (0.70700002f / horizontal);
        lighting->shadow_vector.n[1] = shadow_y * (0.70700002f / horizontal);
    }
    else
    {
        lighting->shadow_vector.n[2] = -sqrtf(1.0f - (horizontal * horizontal));
    }

    /* shadow color: darkened toward the lightmap, floored at the ambient base */
    float accuracy_term = (1.0f - radiosity_accuracy) * 0.5f;
    lighting->shadow_color.n[0] = clamp_lo(accuracy_term - ((lighting->distant_lights[0].color.n[0] * 1.3f) - 1.0f),
                                           object_light_ambient_base);
    lighting->shadow_color.n[1] = clamp_lo(-((lighting->distant_lights[0].color.n[1] * 1.3f) - 1.0f) + accuracy_term,
                                           object_light_ambient_base);
    lighting->shadow_color.n[2] = clamp_lo(-((lighting->distant_lights[0].color.n[2] * 1.3f) - 1.0f) + accuracy_term,
                                           object_light_ambient_base);

    if ( (flags & (1u << _distant_lighting_brighten_bit)) != 0 )
    {
        brighten_real_rgb_color(&lighting->ambient_color, 0.2f);
        brighten_real_rgb_color(&lighting->distant_lights[0].color, 0.30000001f);
        brighten_real_rgb_color(&lighting->distant_lights[1].color, 0.2f);
        brighten_real_rgb_color((real_rgb_color *)&lighting->reflection_tint_color.n[1], 0.5f);
        lighting->reflection_tint_color.n[0] = 1.0f;
    }
}
