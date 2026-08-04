/* shader_texture_animation_evaluate @0x837559A0 — compute the two rows of a 2D texture-coordinate transform
 * (rotation + scale + translation) from a shader's texture-animation parameters at a given time. Each axis
 * (u, v, rotation) animates a periodic function whose amplitude is optionally modulated by a runtime input
 * (render_animation->values, 1-based via the *_source fields). The rotation angle is in degrees and converted
 * to radians (0/360 short-circuit to identity).
 *
 * DEVIATION: the database decompilation reported a failed local-variable allocation, so its register/local
 * naming is unreliable; this body is reconstructed from the algebraic structure and the (type-driven, hence
 * reliable) struct field accesses. The matrix scale columns use the caller's u_scale/v_scale; the per-axis
 * translation amplitudes use the shader_texture_animation u_scale/v_scale/r_scale. */

#include <stdint.h>
#include <math.h>
#include "headers/shader_texture_animation.h"
#include "headers/render_animation.h"
#include "headers/real_vector4d.h"
#include "headers/math_constants.h"

#define DEGREES_TO_RADIANS DEG_TO_RAD

extern float periodic_function_evaluate(int16_t function_type, float time);

void shader_texture_animation_evaluate(
        const shader_texture_animation *texture_animation,
        const render_animation *render_animation,
        float u_scale,
        float v_scale,
        float u_offset,
        float v_offset,
        float r_offset,
        float time_value,
        real_vector4d *u_transform_reference,
        real_vector4d *v_transform_reference)
{
    float u_period = texture_animation->u_period != 0.0f ? texture_animation->u_period : 1.0f;
    float v_period = texture_animation->v_period != 0.0f ? texture_animation->v_period : 1.0f;
    float r_period = texture_animation->r_period != 0.0f ? texture_animation->r_period : 1.0f;

    float u_input = 1.0f;
    float v_input = 1.0f;
    float r_input = 1.0f;
    if ( render_animation )
    {
        const float *values = render_animation->values;
        if ( texture_animation->u_source )
            u_input = values[texture_animation->u_source - 1];
        if ( texture_animation->v_source )
            v_input = values[texture_animation->v_source - 1];
        if ( texture_animation->r_source )
            r_input = values[texture_animation->r_source - 1];
    }

    float u_translation = periodic_function_evaluate(texture_animation->u_function,
                              (texture_animation->u_phase + time_value) / u_period)
                          * texture_animation->u_scale * u_input;
    float v_translation = periodic_function_evaluate(texture_animation->v_function,
                              (texture_animation->v_phase + time_value) / v_period)
                          * texture_animation->v_scale * v_input;
    float rotation_degrees = periodic_function_evaluate(texture_animation->r_function,
                                 (texture_animation->r_phase + time_value) / r_period)
                             * texture_animation->r_scale * r_input;

    float total_u = (u_offset - texture_animation->r_center.n[0]) + u_translation;
    float total_v = (v_offset - texture_animation->r_center.n[1]) + v_translation;

    float angle_degrees = rotation_degrees + r_offset;
    float cosine, sine;
    if ( angle_degrees == 0.0f || angle_degrees == 360.0f )
    {
        cosine = 1.0f;
        sine = 0.0f;
    }
    else
    {
        float radians = angle_degrees * DEGREES_TO_RADIANS;
        cosine = cos(radians);
        sine = sin(radians);
    }

    u_transform_reference->n[2] = 0.0f;
    u_transform_reference->n[0] = cosine * u_scale;
    u_transform_reference->n[1] = -(sine * v_scale);
    u_transform_reference->n[3] = (cosine * total_u - sine * total_v) + texture_animation->r_center.n[0];

    v_transform_reference->n[0] = sine * u_scale;
    v_transform_reference->n[1] = cosine * v_scale;
    v_transform_reference->n[2] = 0.0f;
    v_transform_reference->n[3] = (cosine * total_v + sine * total_u) + texture_animation->r_center.n[1];
}
