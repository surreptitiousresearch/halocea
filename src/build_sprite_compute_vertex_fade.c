#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/shader_framebuffer_fade_mode.h"

extern float __fabs(float x);
extern float __fsqrts(float x);

float build_sprite_compute_vertex_fade(int16_t fade_mode, const real_point3d *viewer_point, const real_vector3d *viewer_normal)
{
    float fade = 1.0f;

    if ( fade_mode != _shader_framebuffer_fade_mode_none )
    {
        float dot = viewer_point->n[0] * viewer_normal->n[0]
                  + viewer_point->n[1] * viewer_normal->n[1]
                  + viewer_point->n[2] * viewer_normal->n[2];
        float length = __fsqrts(viewer_point->n[0] * viewer_point->n[0]
                               + viewer_point->n[1] * viewer_point->n[1]
                               + viewer_point->n[2] * viewer_point->n[2]);
        float cosine = __fabs(dot / length);

        fade = (fade_mode == _shader_framebuffer_fade_mode_fade_when_parallel) ? (1.0f - cosine) : cosine;
    }
    return fade;
}
