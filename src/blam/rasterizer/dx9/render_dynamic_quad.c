/* render_dynamic_quad @0x8382C9E8 — render a single 4-vertex quad (two triangles) through the model
 * rasterizer using a temporary dynamic vertex/triangle buffer. Copies the four caller-supplied vertices into
 * a locked dynamic vertex buffer (forcing both skin weights to 0.5), builds the 0-1-2 / 2-3-0 triangle fan,
 * resolves the shader tag, fills a rasterizer_model_begin_parameters (identity single-node skinning; caller
 * lighting or a white/one-distant-shadow default; caller animation or the global default color/value tables;
 * base-map UV scale from u_scale/v_scale; centroid = average of the four positions), and dispatches to the
 * transparent-geometry submit path or the opaque model-draw path by shader type.
 *
 * DEVIATIONS (disasm-verified, 0x8382C9E8-0x8382CF04):
 *  - The transparent submit's sort_filth argument is nullptr: disasm passes r26, which is the same register
 *    that zeroes rasterizer_globals.current_lock_operation at cleanup (r26 == 0). The decompiler lost it as an
 *    uninitialized local (v26).
 *  - u_scale/v_scale are float (DB prototype); the FPR-shadow ABI widened them to double in the decompiler.
 *  - rasterizer_model_end / rasterizer_models_end are void (corpus convention; the DB has no prototype and the
 *    disasm sets up no argument between them — the decompiler's v25 return-threading is a stale-r3 artifact). */

#include <stdint.h>
#include <string.h>
#include "headers/model_vertex_uncompressed.h"
#include "headers/rasterizer_model_begin_parameters.h"
#include "headers/rasterizer_triangle.h"
#include "headers/shader.h"
#include "headers/render_lighting.h"
#include "headers/render_animation.h"
#include "headers/render_sort_filth.h"
#include "headers/real_point3d.h"
#include "headers/real_matrix4x3.h"
#include "headers/real_argb_color.h"
#include "headers/real_rgb_color.h"
#include "headers/rasterizer_globals.h"
#include "headers/global_tag_instances.h"
#include "headers/blam_data_globals.h"
#include "headers/rasterizer_lock_operation.h"
#include "headers/rasterizer_vertex_type.h"

struct triangle_buffer;
struct vertex_buffer;
struct transparent_geometry_group;


#include "headers/triangle_buffer.h"
#include "headers/vertex_buffer.h"
#include "headers/render_sort_filth.h"
/* memcpy declared by <string.h> */
extern int rasterizer_dynamic_triangles_new(int count);
extern int rasterizer_dynamic_vertices_new(int16_t type, int count);
extern void * rasterizer_dynamic_vertices_lock(int dynamic_vertex_buffer_index);
extern rasterizer_triangle *rasterizer_dynamic_triangles_lock(int dynamic_triangle_buffer_index);
extern void rasterizer_dynamic_triangles_unlock(int dynamic_triangle_buffer_index);
extern void rasterizer_dynamic_vertices_unlock(int dynamic_vertex_buffer_index);
extern void rasterizer_dynamic_triangles_delete(int dynamic_triangle_buffer_index);
extern void rasterizer_dynamic_vertices_delete(int dynamic_vertex_buffer_index);
extern uint8_t shader_type_is_transparent(int16_t shader_type);
extern void rasterizer_profile_enable(uint8_t enable);
extern void rasterizer_models_begin(uint8_t sky);
extern void rasterizer_model_begin(const rasterizer_model_begin_parameters *parameters, uint8_t do_not_change_z_stencil_states);
extern void rasterizer_model_transparent_geometry_submit(const shader *shader, int16_t shader_permutation_index, const triangle_buffer *triangle_buffer, int dynamic_triangle_buffer_index, int triangle_count, const vertex_buffer *vertex_buffer, int dynamic_vertex_buffer_index, const real_point3d *centroid, render_sort_filth *sort_filth);
extern void rasterizer_model_draw(const shader *shader, int16_t shader_permutation_index, const triangle_buffer *triangle_buffer, int dynamic_triangle_buffer_index, int triangle_count, const vertex_buffer *vertex_buffer, int dynamic_vertex_buffer_index);
extern void rasterizer_model_end(void);
extern void rasterizer_models_end(void);

void render_dynamic_quad(model_vertex_uncompressed *verts, int shader_index, const render_lighting *lighting,
        const render_animation *animation, float u_scale, float v_scale)
{
    rasterizer_globals.current_lock_operation = _rasterizer_lock_koth;

    int dynamic_triangle_buffer_index = rasterizer_dynamic_triangles_new(2);
    int dynamic_vertex_buffer_index = rasterizer_dynamic_vertices_new(_rasterizer_vertex_type_model_uncompressed, 4);
    if ( dynamic_triangle_buffer_index == -1 || dynamic_vertex_buffer_index == -1 )
    {
        rasterizer_globals.current_lock_operation = 0;
        return;
    }

    model_vertex_uncompressed *dst_verts =
            rasterizer_dynamic_vertices_lock(dynamic_vertex_buffer_index);
    rasterizer_triangle *triangles = rasterizer_dynamic_triangles_lock(dynamic_triangle_buffer_index);

    /* copy the four vertices, forcing the skin weights to 0.5 (single identity node) */
    for ( int i = 0; i < 4; ++i )
    {
        dst_verts[i] = verts[i];
        dst_verts[i].weights[0] = 0.5f;
        dst_verts[i].weights[1] = 0.5f;
    }

    triangles[0].vertex_indices[0] = 0;
    triangles[0].vertex_indices[1] = 1;
    triangles[0].vertex_indices[2] = 2;
    triangles[1].vertex_indices[0] = 2;
    triangles[1].vertex_indices[1] = 3;
    triangles[1].vertex_indices[2] = 0;
    rasterizer_dynamic_triangles_unlock(dynamic_triangle_buffer_index);
    rasterizer_dynamic_vertices_unlock(dynamic_vertex_buffer_index);

    const shader *quad_shader = TAG_GET(const shader, shader_index);

    real_point3d centroid;
    centroid.n[0] = (((verts[0].position.n[0] + verts[3].position.n[0])
                    + verts[2].position.n[0]) + verts[1].position.n[0]) * (float)0.25;
    centroid.n[1] = (((verts[3].position.n[1] + verts[2].position.n[1])
                    + verts[1].position.n[1]) + verts[0].position.n[1]) * (float)0.25;
    centroid.n[2] = (((verts[3].position.n[2] + verts[2].position.n[2])
                    + verts[1].position.n[2]) + verts[0].position.n[2]) * (float)0.25;

    rasterizer_model_begin_parameters parameters;
    memset(&parameters, 0, sizeof(parameters));
    parameters.unique_id = 1;
    parameters.skinning.node_matrix_count = 1;
    parameters.skinning.node_matrices = (real_matrix4x3 *)global_identity4x3;
    if ( lighting )
    {
        memcpy(&parameters.lighting, lighting, sizeof(parameters.lighting));
    }
    else
    {
        parameters.lighting.point_light_count = 0;
        parameters.lighting.distant_light_count = 0;
        parameters.lighting.ambient_color.n[0] = global_real_rgb_white->n[0];
        parameters.lighting.ambient_color.n[1] = global_real_rgb_white->n[1];
        parameters.lighting.ambient_color.n[2] = global_real_rgb_white->n[2];
        parameters.lighting.reflection_tint_color = *global_real_argb_white;
        parameters.lighting.shadow_vector.n[0] = 0.0f;
        parameters.lighting.shadow_vector.n[1] = 1.0f;
        parameters.lighting.shadow_vector.n[2] = 0.0f;
        parameters.lighting.shadow_color.n[0] = global_real_rgb_black->n[0];
        parameters.lighting.shadow_color.n[1] = global_real_rgb_black->n[1];
        parameters.lighting.shadow_color.n[2] = global_real_rgb_black->n[2];
    }
    if ( animation )
    {
        parameters.animation = *animation;
    }
    else
    {
        parameters.animation.colors = global_default_animation_colors;
        parameters.animation.values = global_default_animation_values;
    }
    parameters.base_map_scale.n[0] = u_scale;
    parameters.base_map_scale.n[1] = v_scale;
    parameters.centroid = centroid;

    rasterizer_profile_enable(0);
    rasterizer_models_begin(0);
    rasterizer_model_begin(&parameters, 1u);
    if ( shader_type_is_transparent(quad_shader->base.type) )
        rasterizer_model_transparent_geometry_submit(quad_shader, 0, nullptr,
                dynamic_triangle_buffer_index, 2, nullptr, dynamic_vertex_buffer_index, &centroid, nullptr);
    else
        rasterizer_model_draw(quad_shader, 0, nullptr, dynamic_triangle_buffer_index, 2, nullptr,
                dynamic_vertex_buffer_index);
    rasterizer_model_end();
    rasterizer_models_end();
    rasterizer_profile_enable(1u);
    rasterizer_dynamic_triangles_delete(dynamic_triangle_buffer_index);
    rasterizer_dynamic_vertices_delete(dynamic_vertex_buffer_index);
    rasterizer_globals.current_lock_operation = 0;
}
