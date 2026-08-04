/* rasterizer_set_model_lighting_distant_light @0x8369CE40 — stage one distant (directional) light
 * into a model-lighting vertex-shader constant block. A non-null light copies its direction into the
 * slot's forward vector and its color into the slot's color; a null light zeroes the whole 32-byte
 * distant-light constant slot (8 floats). */

#include <stdint.h>
#include "headers/render_distant_light.h"
#include "headers/rasterizer_model_lighting_constants.h"

void rasterizer_set_model_lighting_distant_light(const render_distant_light *light, int16_t light_num, rasterizer_model_lighting_constants *lighting_constants)
{
    if (light)
    {
        lighting_constants->distant_lights[light_num].forward = light->direction;
        lighting_constants->distant_lights[light_num].color = light->color;
    }
    else
    {
        /* Zero the entire distant_lights[light_num] slot (rasterizer_distant_light_constants == 8 floats).
         * The decompiler expressed this as a pointer walk from point_lights[1].spot_falloff_coefficient_B. */
        float *slot = (float *)&lighting_constants->distant_lights[light_num];
        for (int i = 0; i < 8; i++)
            slot[i] = 0.0f;
    }
}
