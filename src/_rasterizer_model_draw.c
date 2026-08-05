#include <stdint.h>
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/rasterizer_model_begin_parameters.h"
#include "headers/shader.h"
#include "headers/shader_transparent_plasma.h"
#include "headers/triangle_buffer.h"
#include "headers/vertex_buffer.h"
#include "headers/transparent_geometry_group.h"
#include "headers/render_sort_filth.h"
#include "headers/real_point3d.h"
#include "headers/shader_type.h"
#include "headers/render_model_effect_type.h"
#include "headers/blam_data_globals.h"

/* DEVIATION: DB proto says void; decompiler uses return value as transparent_geometry_group*.
 * Two distinct callees exist (disasm 0x8378D348 vs 0x8378D3A4): the internal
 * `_rasterizer_model_transparent_geometry_submit` (return captured) and the public
 * `rasterizer_model_transparent_geometry_submit` (return ignored). */
#include "headers/real_point3d.h"
extern transparent_geometry_group *_rasterizer_model_transparent_geometry_submit(const shader *shader, int16_t shader_permutation_index, const triangle_buffer *triangle_buffer, int dynamic_triangle_buffer_index, int triangle_count, const vertex_buffer *vertex_buffer, int dynamic_vertex_buffer_index, const real_point3d *centroid, render_sort_filth *sort_filth);
extern void rasterizer_model_transparent_geometry_submit(const shader *shader, int16_t shader_permutation_index, const triangle_buffer *triangle_buffer, int dynamic_triangle_buffer_index, int triangle_count, const vertex_buffer *vertex_buffer, int dynamic_vertex_buffer_index, const real_point3d *centroid, render_sort_filth *sort_filth);
extern void *rasterizer_memory_alloc(const void *src, unsigned int size);
extern void rasterizer_active_camouflage_set_visibility(uint8_t visibility);
/* DEVIATION: rasterizer_model_draw_environment_shader / _model_shader are function-POINTER globals
 * per DB (see blam_data_globals.h); their extern function declarations were removed to avoid the
 * variable-vs-function collision — the indirect calls below are unchanged. */

void _rasterizer_model_draw(
        const shader *shader_ref, /* renamed from `shader` — shadowed the `shader` typedef */
        int16_t shader_permutation_index,
        const triangle_buffer *triangle_buffer,
        int dynamic_triangle_buffer_index,
        int triangle_count,
        const vertex_buffer *vertex_buffer,
        int dynamic_vertex_buffer_index)
{
    if (!rasterizer_debug_options.draw_models)
        return;

    render_sort_filth *sort_filth = 0;
    const shader *modifier_shader = local_parameters->effect.modifier_shader;
    if (modifier_shader)
    {
        int should_submit = 1;
        if (modifier_shader->base.type == _shader_type_transparent_plasma)
        {
            /* DEVIATION: decompiler punned the lhz+extsh at shader+0x2C as float-bits>>16; disasm
             * reads the int16 shader_transparent_plasma.plasma.intensity_source (DB-verified). */
            int16_t intensity_source = ((const shader_transparent_plasma *)modifier_shader)->plasma.intensity_source;
            if (intensity_source >= 1 && intensity_source <= 4)
            {
                const float *values = local_parameters->effect.modifier_animation.values;
                if (values)
                    should_submit = values[intensity_source - 1] != 0.0f;
            }
        }
        if (should_submit)
        {
            /* disasm 0x8378D348 targets the internal underscore variant (return captured) */
            transparent_geometry_group *group = _rasterizer_model_transparent_geometry_submit(
                    modifier_shader, shader_permutation_index,
                    triangle_buffer, dynamic_triangle_buffer_index,
                    triangle_count, vertex_buffer, dynamic_vertex_buffer_index,
                    &local_parameters->centroid, sort_filth);
            if (group)
            {
                const render_animation *anim = rasterizer_memory_alloc(
                        &local_parameters->effect.modifier_animation, 8);
                group->animation = anim;
            }
        }
    }

    if (local_model_effect_type == _render_model_effect_type_active_camouflage)
    {
        rasterizer_model_transparent_geometry_submit(
                shader_ref, shader_permutation_index,
                triangle_buffer, dynamic_triangle_buffer_index,
                triangle_count, vertex_buffer, dynamic_vertex_buffer_index,
                &local_parameters->centroid, sort_filth);
        rasterizer_active_camouflage_set_visibility(1);
    }
    else if (local_model_effect_type == _render_model_effect_type_none)
    {
        if (shader_ref->base.type == _shader_type_environment)
            rasterizer_model_draw_environment_shader(
                    shader_ref, shader_permutation_index,
                    triangle_buffer, dynamic_triangle_buffer_index,
                    triangle_count, vertex_buffer, dynamic_vertex_buffer_index);
        else
            rasterizer_model_draw_model_shader(
                    shader_ref, shader_permutation_index,
                    triangle_buffer, dynamic_triangle_buffer_index,
                    triangle_count, vertex_buffer, dynamic_vertex_buffer_index);
    }
}
