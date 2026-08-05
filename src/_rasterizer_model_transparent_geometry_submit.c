/* _rasterizer_model_transparent_geometry_submit @0x8378CD18 (1124B) — the real implementation behind the
 * `rasterizer_model_transparent_geometry_submit` forwarding wrapper. Allocates (or reuses a singleton)
 * transparent_geometry_group, fills it from the currently-queued `local_parameters` (a
 * rasterizer_model_begin_parameters set up by rasterizer_model_begin) plus this call's own geometry-buffer
 * arguments, computes its camera-relative sort key, and either submits it immediately (skinned/decal-2
 * geometry, drawn through the transparent pass right away) or defers it into the normal sorted queue.
 *
 * DEVIATION — address correction: an earlier revision of the forwarding wrapper recorded this function's
 * address as 0x836EC098 (a stale/mis-copied hex conversion — that address actually falls inside an
 * unrelated function, object_align_marker_to_matrix). The correct address, confirmed via
 * `SELECT address FROM funcs WHERE name='_rasterizer_model_transparent_geometry_submit'`, is 0x8378CD18.
 *
 * DEVIATION — phantom parameters: Hex-Rays inflates this function's signature to 28 parameters (a10..a28),
 * all but the last completely unreferenced in the body — a decompiler stack-scanning artifact, not real
 * arguments (the DB's own `funcs.prototype` lists only the 9 real parameters, matching the wrapper). Only
 * `a28` is real: its use (`*(_DWORD*)a28=0; *(_DWORD*)(a28+4)=0; *(_WORD*)(a28+8)=-1`, later
 * `*(_WORD*)(a28+8)=...; *(_DWORD*)a28=&group->prev...; *(_DWORD*)(a28+4)=&group->next...`) matches
 * render_sort_filth's layout (prev_group_presorted_index_reference@0, next_group_presorted_index_reference@4,
 * group_index@8) field-for-field, confirming it is simply the real 9th parameter `sort_filth`, renumbered
 * by the phantom-parameter inflation. Modeled here with the DB's real 9-parameter signature.
 *
 * DEVIATION — the `render_model_effect` copy: the decompile represents this as a raw 10-iteration
 * pointer-walking loop (`p_shader_permutation_index += 2; *(_DWORD*)p_shader_permutation_index = *p_values;`
 * with `p_values` starting at `&local_parameters->animation.values`, incremented before each read).
 * Address arithmetic confirms both ends resolve to whole-struct copies: destination starts at
 * `&group->shader_permutation_index + 4 bytes` = `&group->effect` (group.effect sits immediately after
 * shader_permutation_index, DB offset 0x14), and source's first read (after the pre-increment) lands at
 * `&local_parameters->animation.values + 4 bytes` = `&local_parameters->effect` (DB offset 140 = animation's
 * offset 132 + animation.values' offset 4 + 4). 10 iterations * 4 bytes = 40 bytes = sizeof(render_model_effect)
 * exactly. Reproduced as a direct struct assignment. */

#include <stdint.h>
#include "headers/shader.h"
#include "headers/triangle_buffer.h"
#include "headers/vertex_buffer.h"
#include "headers/real_point3d.h"
#include "headers/render_sort_filth.h"
#include "headers/transparent_geometry_group.h"
#include "headers/rasterizer_model_begin_parameters.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/rasterizer_window_begin_parameters.h"
#include "headers/rasterizer_geometry_flags.h"
#include "headers/shader_type.h"
#include "headers/shader_model_flags.h"
#include "headers/render_model_effect_type.h"
#include "headers/blam_data_globals.h"


extern int transparent_geometry_cached_sorted_index;                    /* decal/camouflage singleton group's cached sorted index */
extern const render_animation *transparent_geometry_cached_animation;  /* cached queued-parameters animation snapshot */
extern const real_matrix4x3 *transparent_geometry_cached_node_matrices;    /* cached queued-parameters node matrices */
extern int16_t transparent_geometry_cached_node_matrix_count;                    /* cached queued-parameters node matrix count */
extern const render_lighting *transparent_geometry_cached_lighting;    /* cached queued-parameters lighting snapshot */

extern uint8_t shader_is_decal(const shader *shader);
extern transparent_geometry_group *rasterizer_transparent_geometry_new_group(void);
extern transparent_geometry_group *rasterizer_transparent_geometry_new_group2(void);
extern int16_t rasterizer_transparent_geometry_get_group_presorted_index(const transparent_geometry_group *group);
extern void rasterizer_transparent_geometry_groups_begin(void);
extern void rasterizer_transparent_geometry_group_draw(const transparent_geometry_group *group, uint8_t dirty);
extern void rasterizer_transparent_geometry_groups_end(void);
extern const void *rasterizer_memory_alloc_const(const void *src, unsigned int size);

transparent_geometry_group *_rasterizer_model_transparent_geometry_submit(
        const shader *shader,
        int16_t shader_permutation_index,
        const triangle_buffer *triangle_buffer,
        int dynamic_triangle_buffer_index,
        int triangle_count,
        const vertex_buffer *vertex_buffer,
        int dynamic_vertex_buffer_index,
        const real_point3d *centroid,
        render_sort_filth *sort_filth)
{
    transparent_geometry_group *new_group = 0;

    if (!rasterizer_debug_options.draw_models || !rasterizer_debug_options.draw_model_transparent_geometry)
        return new_group;

    /* skip_submit: true only when shader is a decal-capable shader (type 4) whose second radiosity block
     * has flag bit 0x8 set — those are drawn through a different path entirely. */
    /* second radiosity block of a model shader overlays shader_model.flags; bit 3 = alpha-blended decal (DB enum shader_model_flags). */
    uint8_t skip_submit = shader && shader->base.type == _shader_type_model && (shader[1].base.radiosity.flags & (1u << _shader_model_alpha_blended_decal_bit)) != 0;

    /* wants_skinned_or_decal_path: camouflage/skinned effect, or a non-decal shader while the current
     * model effect is camouflage (type 1) — both gate the immediate-draw / singleton-group branch below. */
    uint8_t wants_skinned_or_decal_path = local_model_effect_type != _render_model_effect_type_active_camouflage
            || (shader && shader->base.type == _shader_type_model && shader[1].base.radiosity.flags);

    if (skip_submit)
    {
        if (sort_filth)
        {
            sort_filth->prev_group_presorted_index_reference = 0;
            sort_filth->next_group_presorted_index_reference = 0;
            sort_filth->group_index = -1;
        }
        return 0;
    }

    const rasterizer_model_begin_parameters *parameters = local_parameters;
    unsigned int geometry_flags = local_parameters->geometry_flags;
    if (wants_skinned_or_decal_path)
    {
        uint8_t is_decal = shader_is_decal(shader);
        parameters = local_parameters;
        if (is_decal)
            geometry_flags |= (1u << _rasterizer_geometry_no_sort_bit)
                            | (1u << _rasterizer_geometry_no_queue_bit);
    }

    transparent_geometry_group *group;
    if (wants_skinned_or_decal_path && (geometry_flags & (1u << _rasterizer_geometry_no_queue_bit)) != 0)
    {
        transparent_geometry_cached_sorted_index = -1;
        group = &immediate_transparent_geometry_group;
    }
    else
    {
        if (local_model_effect_type == _render_model_effect_type_active_camouflage && shader && shader->base.type != _shader_type_model)
            group = rasterizer_transparent_geometry_new_group2();
        else
        {
            new_group = rasterizer_transparent_geometry_new_group();
            group = new_group;
        }

        if (sort_filth)
        {
            sort_filth->group_index = rasterizer_transparent_geometry_get_group_presorted_index(group);
            sort_filth->prev_group_presorted_index_reference = &group->prev_group_presorted_index;
            sort_filth->next_group_presorted_index_reference = &group->next_group_presorted_index;
        }

        if (!group)
        {
            if (!warned_3)
                warned_3 = 1;
            return new_group;
        }
        parameters = local_parameters;
    }

    group->geometry_flags = geometry_flags;
    group->object_index = parameters->unique_id;

    if (geometry_flags & (1u << _rasterizer_geometry_parts_define_local_nodes_bit))
    {
        group->local_node_remap_table = g_CurrentLocalNodeTable;
        group->local_node_remap_table_size = g_CurrentLocalNodeTableSize;
    }
    else
    {
        group->local_node_remap_table = 0;
        group->local_node_remap_table_size = 0;
    }

    if (parameters->effect.type)
    {
        group->source_object_index = parameters->effect.source_object_index;
        group->centroid = parameters->effect.source_object_centroid;
    }
    else
    {
        group->source_object_index = 0;
        group->centroid = *centroid;
    }

    group->shader = shader;
    group->shader_permutation_index = shader_permutation_index;
    group->effect = parameters->effect;
    group->triangle_count = triangle_count;
    group->dynamic_vertex_buffer_index = dynamic_vertex_buffer_index;
    group->vertex_buffers = vertex_buffer;
    group->dynamic_triangle_buffer_index = dynamic_triangle_buffer_index;
    group->triangle_buffer = triangle_buffer;
    group->first_triangle_index = 0;
    group->lightmap = 0;

    float camera_relative_x = group->centroid.n[0] - global_window_parameters.camera.position.n[0];
    float camera_relative_y = group->centroid.n[1] - global_window_parameters.camera.position.n[1];
    float camera_relative_z = group->centroid.n[2] - global_window_parameters.camera.position.n[2];
    group->plane.n.n[0] = 0.0f;
    group->z_sort = -(global_window_parameters.camera.forward.n[1] * camera_relative_y
            + (global_window_parameters.camera.forward.n[2] * camera_relative_z
             + global_window_parameters.camera.forward.n[0] * camera_relative_x));
    group->plane.n.n[1] = 0.0f;
    group->plane.n.n[2] = 0.0f;
    group->plane.d = 0.0f;
    group->model_base_map_scale = parameters->base_map_scale;
    group->prev_group_presorted_index = -1;
    group->next_group_presorted_index = -1;

    if ((uint16_t)local_model_effect_type != _render_model_effect_type_active_camouflage || shader->base.type == _shader_type_model)
        group->active_camouflage_transparent_source_object_index = 0;
    else
        group->active_camouflage_transparent_source_object_index = parameters->effect.source_object_index;

    group->cortana_hack = rasterizer_model_cortana_hack;

    if (geometry_flags & (1u << _rasterizer_geometry_no_queue_bit))
    {
        group->node_matrices = parameters->skinning.node_matrices;
        group->lighting = &parameters->lighting;
        group->animation = &parameters->animation;
        group->node_matrix_count = parameters->skinning.node_matrix_count;
        rasterizer_transparent_geometry_groups_begin();
        rasterizer_transparent_geometry_group_draw(group, 0);
        rasterizer_transparent_geometry_groups_end();
        local_pixel_shader_dirty_flag = 1;
        return new_group;
    }

    if (!local_parameters_queued_flag)
    {
        transparent_geometry_cached_node_matrices = (const real_matrix4x3 *)rasterizer_memory_alloc_const(
                parameters->skinning.node_matrices, 52 * parameters->skinning.node_matrix_count);
        transparent_geometry_cached_node_matrix_count = local_parameters->skinning.node_matrix_count;
        transparent_geometry_cached_lighting = (const render_lighting *)rasterizer_memory_alloc_const(&local_parameters->lighting, 0x74u);
        transparent_geometry_cached_animation = (const render_animation *)rasterizer_memory_alloc_const(&local_parameters->animation, 8u);
        local_parameters_queued_flag = 1;
    }
    group->animation = transparent_geometry_cached_animation;
    group->node_matrices = transparent_geometry_cached_node_matrices;
    group->node_matrix_count = transparent_geometry_cached_node_matrix_count;
    group->lighting = transparent_geometry_cached_lighting;
    return new_group;
}
