/* _rasterizer_dynamic_unlit_geometry_draw @0x837A8CA8 — queue a dynamic (non-model) unlit geometry batch as
 * a transparent_geometry_group. z_sort gets a +0.25 nudge for an effect shader that asks for a sort bias.
 *
 * DEVIATION: the decompiler folded the flag read into `shader[1].base.radiosity.flags` on the 40-byte base,
 * which named the wrong struct AND the wrong bit. `lhz r5, 0x24(r29)` / `cmplwi 1` @0x837A8DF4 is
 * base.type == _shader_type_effect (type 1 is effect, not model), and `lhz r10, 0x28(r29)` /
 * `clrlwi r9, r10, 31` @0x837A8E00 is shader_effect's own uint16_t flags at +0x00 of its derived body,
 * bit 0 = _shader_effect_sort_bias_bit — so the +0.25 (`__real_3e800000`) is exactly the sort bias the
 * flag names, not an odd condition. */

#include "headers/transparent_geometry_group.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/rasterizer_window_begin_parameters.h"
#include "headers/shader.h"
#include "headers/bitmap_data.h"
#include "headers/render_animation.h"
#include "headers/shader_type.h"
#include "headers/shader_effect.h"
#include "headers/shader_effect_flags.h"
#include "headers/blam_data_globals.h"


extern transparent_geometry_group *rasterizer_transparent_geometry_new_group(void);

void _rasterizer_dynamic_unlit_geometry_draw(const shader *shader, const bitmap_data *primary_map,
                                              const render_animation *animation, int dynamic_triangle_buffer_index,
                                              int dynamic_vertex_buffer_index, int triangle_count,
                                              const real_point3d *centroid, unsigned int geometry_flags)
{
    if ( !rasterizer_debug_options.draw_dynamic_unlit_geometry )
        return;

    float camera_relative_x = centroid->n[0] - global_window_parameters.camera.position.n[0];
    float camera_relative_y = centroid->n[1] - global_window_parameters.camera.position.n[1];
    float camera_relative_z = centroid->n[2] - global_window_parameters.camera.position.n[2];

    transparent_geometry_group *group = rasterizer_transparent_geometry_new_group();
    if ( !group )
    {
        if ( !warned_8 )
            warned_8 = 1;
        return;
    }

    group->geometry_flags = geometry_flags;
    group->shader = shader;
    group->object_index = 0;
    group->source_object_index = 0;
    group->shader_permutation_index = 0;
    group->effect.type = 0;
    group->dynamic_triangle_buffer_index = dynamic_triangle_buffer_index;
    group->triangle_buffer = 0;
    group->first_triangle_index = 0;
    group->triangle_count = triangle_count;
    group->dynamic_vertex_buffer_index = dynamic_vertex_buffer_index;
    group->vertex_buffers = 0;
    group->lightmap = primary_map;

    float z_sort = -(global_window_parameters.camera.forward.n[1] * camera_relative_y
                    + global_window_parameters.camera.forward.n[0] * camera_relative_x
                    + global_window_parameters.camera.forward.n[2] * camera_relative_z);
    group->z_sort = z_sort;

    group->centroid = *centroid;
    group->model_base_map_scale.n[0] = 1.0f;
    group->model_base_map_scale.n[1] = 1.0f;
    group->plane.n.n[0] = 0.0f;
    group->plane.n.n[1] = 0.0f;
    group->plane.n.n[2] = 0.0f;
    group->plane.d = 0.0f;
    group->prev_group_presorted_index = -1;
    group->next_group_presorted_index = -1;
    group->active_camouflage_transparent_source_object_index = 0;
    group->cortana_hack = 0;

    if ( shader->base.type == _shader_type_effect
         && (((const shader_effect *)shader)->effect.flags & (1u << _shader_effect_sort_bias_bit)) != 0 )
        group->z_sort = z_sort + 0.25f;

    group->node_matrices = 0;
    group->node_matrix_count = 0;
    group->lighting = 0;
    group->animation = 0;
}
