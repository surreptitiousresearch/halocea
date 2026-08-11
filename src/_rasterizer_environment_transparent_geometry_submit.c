/* _rasterizer_environment_transparent_geometry_submit @0x837A8398 0x837A8398 — queue (or immediately draw) one transparent
 * environment geometry batch. Builds a transparent_geometry_group from the shader + its triangle/vertex buffers,
 * computes the back-to-front sort key from the centroid relative to the camera, snapshots the clip plane and a
 * copy of the lighting block, and then: for water shaders forces a visibility flag and, when that water shader
 * asks to be drawn before fog, draws + marks the group pending immediately; for decals / flag-2 geometry
 * it draws immediately; otherwise the freshly allocated group is left queued for the sorted transparent pass.
 *
 * DEVIATION (decompiler "stack over-read"): the decompiler could not bound the stack parameter area (this impl
 * is only ever reached through a function-pointer dispatch table, so there is no direct caller to constrain it)
 * and invented phantom parameters a13..a34. Only three stack arguments are actually read — at frame offsets
 * 0x54 = plane (pointer, NULL ⇒ no clip plane), 0x64 = render_lighting source (copied via alloc_const, 0x74
 * bytes), and 0x6C = geometry_flags. The `offset` parameter (a real_vector3d passed by value, occupying the gap
 * at 0x58..0x60) is unused in this build. The database's 12-parameter prototype is the canonical interface and
 * is used verbatim. */

#include <stdint.h>
#include "headers/transparent_geometry_group.h"
#include "headers/shader.h"
#include "headers/bitmap_data.h"
#include "headers/vertex_buffer.h"
#include "headers/real_plane3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_point3d.h"
#include "headers/render_lighting.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/rasterizer_window_begin_parameters.h"
#include "headers/rasterizer_geometry_flags.h"
#include "headers/shader_type.h"
#include "headers/shader_transparent_water.h"
#include "headers/shader_transparent_water_flags.h"
#include "headers/blam_data_globals.h"


extern uint8_t shader_is_decal(const shader *shader);
extern const void *rasterizer_memory_alloc_const(const void *src, unsigned int size);
extern transparent_geometry_group *rasterizer_transparent_geometry_new_group(void);
extern void rasterizer_transparent_geometry_group_draw(const transparent_geometry_group *group, uint8_t dirty);
extern void rasterizer_transparent_geometry_set_group_pending_status(const transparent_geometry_group *group, uint8_t status);
extern void rasterizer_water_set_visibility_for_window(uint8_t visibility);

void _rasterizer_environment_transparent_geometry_submit(const shader *shader, int16_t shader_permutation_index, const bitmap_data *lightmap, int dynamic_triangle_buffer_index, int first_triangle_index, int triangle_count, const vertex_buffer *vertex_buffers, const real_point3d *centroid, const real_plane3d *plane, const real_vector3d *offset, const render_lighting *render_lighting, unsigned int geometry_flags)
{
    float to_centroid_x, to_centroid_y, to_centroid_z;
    int flags;
    transparent_geometry_group *group;
    const float *plane_source;
    float zeroed_plane[6];

    if ( !rasterizer_debug_options.draw_environment_transparent_geometry )
        return;

    to_centroid_x = centroid->n[0] - global_window_parameters.camera.position.n[0];
    to_centroid_y = centroid->n[1] - global_window_parameters.camera.position.n[1];
    to_centroid_z = centroid->n[2] - global_window_parameters.camera.position.n[2];

    flags = plane ? (geometry_flags | (1u << _rasterizer_geometry_no_sort_bit)) : geometry_flags;
    if ( shader_is_decal(shader) )
        flags |= (1u << _rasterizer_geometry_no_sort_bit)
               | (1u << _rasterizer_geometry_no_queue_bit)
               | (1u << _rasterizer_geometry_no_fog_bit);

    if ( (flags & (1u << _rasterizer_geometry_no_queue_bit)) != 0 )
    {
        group = &local_group;
        local_group.sorted_index = -1;
    }
    else
    {
        group = rasterizer_transparent_geometry_new_group();
        if ( !group )
        {
            if ( !warned_7 )
                warned_7 = 1;
            return;
        }
    }

    group->geometry_flags = flags;
    group->shader = shader;
    group->object_index = 0;
    group->source_object_index = 0;
    group->effect.type = 0;
    group->shader_permutation_index = shader_permutation_index;
    group->dynamic_triangle_buffer_index = dynamic_triangle_buffer_index;
    group->triangle_buffer = 0;
    group->first_triangle_index = first_triangle_index;
    group->triangle_count = triangle_count;
    group->dynamic_vertex_buffer_index = -1;
    group->vertex_buffers = vertex_buffers;
    group->lightmap = lightmap;
    group->z_sort = -((global_window_parameters.camera.forward.n[1] * to_centroid_y)
                    + ((global_window_parameters.camera.forward.n[0] * to_centroid_x)
                     + (global_window_parameters.camera.forward.n[2] * to_centroid_z)));
    group->centroid.n[0] = centroid->n[0];
    group->centroid.n[1] = centroid->n[1];
    group->centroid.n[2] = centroid->n[2];

    /* snapshot the clip plane (4 floats), or a zeroed plane when none was supplied */
    if ( plane )
    {
        plane_source = (const float *)plane;
    }
    else
    {
        zeroed_plane[0] = 0.0f;
        zeroed_plane[1] = 0.0f;
        zeroed_plane[2] = 0.0f;
        zeroed_plane[3] = 0.0f;
        plane_source = zeroed_plane;
    }
    group->plane.n.n[0] = plane_source[0];
    group->plane.n.n[1] = plane_source[1];
    group->plane.n.n[2] = plane_source[2];
    group->plane.d = plane_source[3];

    group->model_base_map_scale.n[1] = 1.0f;
    group->model_base_map_scale.n[0] = 1.0f;
    group->prev_group_presorted_index = -1;
    group->next_group_presorted_index = -1;
    group->active_camouflage_transparent_source_object_index = 0;
    group->cortana_hack = 0;
    group->node_matrices = 0;
    group->node_matrix_count = 0;
    group->lighting = (const struct render_lighting *)rasterizer_memory_alloc_const(render_lighting, 0x74);
    group->animation = 0;

    if ( shader->base.type == _shader_type_transparent_water )
        rasterizer_water_set_visibility_for_window(1);

    /* DEVIATION: the decompiler folded this into `shader[1].base.radiosity.flags & 8` on the 40-byte base.
     * `lhz r11, 0x28(r25)` / `rlwinm r10, r11, 0,28,28` @0x837A8590 is the water tag's own uint16_t flags
     * at +0x00 of its derived body, bit 3 — DB enum shader_transparent_water_flags names it. */
    if ( shader->base.type == _shader_type_transparent_water
      && (((const shader_transparent_water *)shader)->water.flags
              & (1u << _shader_transparent_water_draw_before_fog_bit)) != 0 )
    {
        group->geometry_flags |= (1u << _rasterizer_geometry_no_queue_bit);
        rasterizer_transparent_geometry_group_draw(group, 0);
        rasterizer_transparent_geometry_set_group_pending_status(group, 1);
        group->geometry_flags &= ~(1u << _rasterizer_geometry_no_queue_bit);
    }
    else if ( (flags & (1u << _rasterizer_geometry_no_queue_bit)) != 0 )
    {
        rasterizer_transparent_geometry_group_draw(group, 0);
    }
}
