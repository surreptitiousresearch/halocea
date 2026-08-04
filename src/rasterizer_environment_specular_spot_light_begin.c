/* rasterizer_environment_specular_spot_light_begin 0x837A4C08 — fill the point-light vertex-shader constant
 * block for a spot light's specular pass. Builds a right-handed frame from the light's forward and up vectors
 * (the third axis is the normalized up x forward), then writes the negated basis rows, the light position,
 * an inverse-radius scale, and a cone projection scale into vsh_constants__pointlight. The gel map tag index
 * (primary, or secondary when the primary is unresolved) is cached for the draw. No-op when environment
 * specular lights are disabled. */

#include "headers/rasterizer_debug_options_struct.h"
#include "headers/rasterizer_lights.h"
#include "headers/point_light_definition.h"
#include "headers/rasterizer_environment_light_globals.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"

extern float normalize3d(real_vector3d *v);

void rasterizer_environment_specular_spot_light_begin(int light_index)
{
    rasterizer_light_submit_parameters *light;
    point_light_definition *definition;
    int gel_map_index;
    real_vector3d up_cross_forward;
    float radius_scale;
    float cone_scale;
    float forward_x, forward_y, forward_z;
    float up_x, up_y, up_z;

    if ( !rasterizer_debug_options.draw_environment_specular_lights )
        return;

    light = &rasterizer_lights.lights[light_index];
    definition = light->definition;
    gel_map_index = light->definition->gel.map.index;
    if ( gel_map_index == -1 )
        gel_map_index = definition->gel.secondary_map.index;

    forward_x = light->forward.n[0];
    forward_y = light->forward.n[1];
    forward_z = light->forward.n[2];
    up_x = light->up.n[0];
    up_y = light->up.n[1];
    up_z = light->up.n[2];

    /* up x forward */
    up_cross_forward.n[0] = (light->up.n[2] * light->forward.n[1]) - (light->up.n[1] * light->forward.n[2]);
    up_cross_forward.n[2] = (light->forward.n[0] * light->up.n[1]) - (light->up.n[0] * light->forward.n[1]);
    up_cross_forward.n[1] = (light->up.n[0] * light->forward.n[2]) - (light->forward.n[0] * light->up.n[2]);
    normalize3d(&up_cross_forward);

    radius_scale = (definition->geometry.specular_radius_multiplier * light->radius);
    cone_scale = (float)1.0 / (radius_scale - (radius_scale * (float)0.5));

    vsh_constants__pointlight[0] = light->position.n[0];
    vsh_constants__pointlight[6] = -forward_z;
    rasterizer_specular_distance_attenuation_index = gel_map_index;
    vsh_constants__pointlight[2] = light->position.n[2];
    vsh_constants__pointlight[1] = light->position.n[1];
    vsh_constants__pointlight[3] = (float)0.5 / radius_scale;
    vsh_constants__pointlight[7] = 1.0f;
    vsh_constants__pointlight[11] = 1.0f;
    vsh_constants__pointlight[15] = 1.0f;
    vsh_constants__pointlight[8] = -up_cross_forward.n[0];
    vsh_constants__pointlight[4] = -forward_x;
    vsh_constants__pointlight[5] = -forward_y;
    vsh_constants__pointlight[9] = -up_cross_forward.n[1];
    vsh_constants__pointlight[10] = -up_cross_forward.n[2];
    vsh_constants__pointlight[12] = -up_x;
    vsh_constants__pointlight[13] = -up_y;
    vsh_constants__pointlight[14] = -up_z;
    vsh_constants__pointlight[16] = forward_x * cone_scale;
    vsh_constants__pointlight[17] = forward_y * cone_scale;
    vsh_constants__pointlight[18] = forward_z * cone_scale;
    vsh_constants__pointlight[19] = -(cone_scale * (radius_scale * (float)0.5));
}
