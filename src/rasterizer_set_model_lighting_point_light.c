/* rasterizer_set_model_lighting_point_light @0x8369CD38 — stage one collected point light's vertex-shader
 * constants (position, inverse radius^2, forward, color, spot falloff coefficients), or zero the slot if
 * no light was assigned (light_index == -1). */

#include <stdint.h>
#include "headers/rasterizer_model_lighting_constants.h"
#include "headers/rasterizer_lights_t.h"

extern void *memset(void *dest, int value, unsigned int size);

void rasterizer_set_model_lighting_point_light(int light_index, int16_t light_num,
                                               rasterizer_model_lighting_constants *lighting_constants)
{
    rasterizer_point_light_constants *constants = &lighting_constants->point_lights[light_num];

    if ( light_index == -1 )
    {
        memset(constants, 0, sizeof(*constants));
        return;
    }

    rasterizer_light_submit_parameters *light = &rasterizer_lights.lights[light_index];

    constants->position = light->position;
    constants->inverse_radius_squared = 1.0f / (light->radius * light->radius);
    constants->forward = light->forward;
    constants->color = light->color;

    point_light_definition *definition = light->definition;
    if ( definition->geometry.runtime_cosine_falloff_angle == -1.0f )
    {
        constants->spot_falloff_coefficient_B = 1.0f;
        constants->spot_falloff_coefficient_A = 0.0f;
    }
    else
    {
        float inverse_angle_delta = 1.0f / (definition->geometry.runtime_cosine_falloff_angle
                                           - definition->geometry.runtime_cosine_cutoff_angle);
        constants->spot_falloff_coefficient_A = inverse_angle_delta;
        constants->spot_falloff_coefficient_B = -(definition->geometry.runtime_cosine_cutoff_angle * inverse_angle_delta);
    }
}
