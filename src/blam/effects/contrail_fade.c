/* contrail_fade @0x837EA830 — per-point fade factor for a contrail point: fade_mode 0 is fully opaque;
 * otherwise the factor is |cos| of the angle between the (unnormalized) camera→point ray and the point's
 * world normal, optionally shaped through transition function 2 (definition flags bit 6, "fade smoothly"),
 * and inverted (1 - f) for fade_mode 2 (fade when facing). */

#include <stdint.h>
#include "headers/contrail_definition.h"
#include "headers/contrail_definition_flags.h"
#include "headers/render_globals.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/transition_function.h"
#include "headers/shader_framebuffer_fade_mode.h"
#include "headers/blam_data_globals.h"


extern double __fabs(double x);
extern float __fsqrts(float x);
extern float transition_function_evaluate(int16_t function_type, float value);

float contrail_fade(contrail_definition *contrail_definition, int16_t fade_mode, const real_point3d *world_point, const real_vector3d *world_normal)
{
    if ( !fade_mode )
        return 1.0f;

    float to_camera_x = render.camera.position.n[0] - world_point->n[0];
    float to_camera_y = render.camera.position.n[1] - world_point->n[1];
    float to_camera_z = render.camera.position.n[2] - world_point->n[2];

    float fade = __fabs((world_normal->n[0] * to_camera_x
                       + world_normal->n[2] * to_camera_z
                       + world_normal->n[1] * to_camera_y)
                      / __fsqrts(to_camera_x * to_camera_x
                               + to_camera_z * to_camera_z
                               + to_camera_y * to_camera_y));

    if ( (contrail_definition->flags & (1u << _contrail_fades_slowly_bit)) != 0 )
        fade = transition_function_evaluate(_transition_function_very_early, fade);
    if ( fade_mode == _shader_framebuffer_fade_mode_fade_when_parallel )
        fade = 1.0f - fade;
    return fade;
}
