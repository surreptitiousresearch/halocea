/* _rasterizer_environment_specular_light_begin @0x837A4DC0 */
/* _rasterizer_environment_specular_light_begin 0x837A4DC0 — set up the per-light state for an environment
 * specular lighting pass. No-op unless debug drawing is in normal mode and environment specular lights are
 * enabled. A light with a real spot cone (runtime_cosine_falloff_angle != -1) AND at least one gel map takes
 * the spot path (permutation 1, delegates cone/gel setup to ..._specular_spot_light_begin). Otherwise it is
 * treated as a plain point light (permutation 0): the vertex-shader constant block is filled with the light
 * position, an inverse-radius scale, and identity-ish matrix rows, and the distance-attenuation map index is
 * cached. */

#include "headers/rasterizer_debug_options_struct.h"
#include "headers/rasterizer_lights.h"
#include "headers/point_light_definition.h"
#include "headers/game_globals_rasterizer_data.h"
#include "headers/rasterizer_environment_light_globals.h"
#include "headers/real_rgb_color.h"
#include "headers/blam_data_globals.h"

extern float real_rgb_color_brightness(const real_rgb_color *color);
extern void rasterizer_environment_specular_spot_light_begin(int light_index);

void _rasterizer_environment_specular_light_begin(int light_index)
{
    rasterizer_light_submit_parameters *light;
    point_light_definition *definition;
    float brightness;
    float radius_scale;

    if ( rasterizer_debug_options.drawing_mode || !rasterizer_debug_options.draw_environment_specular_lights )
        return;

    light = &rasterizer_lights.lights[light_index];
    definition = light->definition;

    if ( light->definition->geometry.runtime_cosine_falloff_angle == -1.0f
      || (definition->gel.map.index == -1 && definition->gel.secondary_map.index == -1) )
    {
        specular_light_vertex_shader_permutation_index = 0;
        brightness = real_rgb_color_brightness(&rasterizer_lights.lights[light_index].color);
        radius_scale = (light->definition->geometry.specular_radius_multiplier * light->radius);

        vsh_constants__pointlight[0] = light->position.n[0];
        vsh_constants__pointlight[2] = light->position.n[2];
        vsh_constants__pointlight[1] = light->position.n[1];
        environment_specular_light_active = 0;
        vsh_constants__pointlight[7] = 1.0f;
        vsh_constants__pointlight[4] = 0.0f;
        vsh_constants__pointlight[5] = 0.0f;
        vsh_constants__pointlight[6] = 0.0f;
        vsh_constants__pointlight[8] = 0.0f;
        vsh_constants__pointlight[9] = 0.0f;
        vsh_constants__pointlight[10] = 0.0f;
        vsh_constants__pointlight[11] = 1.0f;
        vsh_constants__pointlight[12] = 0.0f;
        vsh_constants__pointlight[13] = 0.0f;
        vsh_constants__pointlight[14] = 0.0f;
        vsh_constants__pointlight[15] = 1.0f;
        vsh_constants__pointlight[16] = 0.0f;
        vsh_constants__pointlight[17] = 0.0f;
        vsh_constants__pointlight[18] = 0.0f;
        vsh_constants__pointlight[19] = 1.0f;
        vsh_constants__pointlight[3] = ((float)1.0 / radius_scale) * (float)0.5;
        rasterizer_specular_distance_attenuation_index = global_rasterizer_data->distance_attenuation.index;
        specular_light_brightness = brightness;
    }
    else
    {
        specular_light_vertex_shader_permutation_index = 1;
        specular_light_brightness = real_rgb_color_brightness(&rasterizer_lights.lights[light_index].color);
        rasterizer_environment_specular_spot_light_begin(light_index);
        environment_specular_light_active = 1;
    }
}
