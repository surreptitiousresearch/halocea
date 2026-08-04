/* _rasterizer_widget_submit @0x83785F40 — queue a widget's pre-built triangle buffer as a transparent
 * draw group (skips all the usual model/skinning/shader setup, since a widget draw call already carries
 * its own geometry). Computes camera-relative depth sort key and stores the centroid, then defaults the
 * base map scale to 1:1 and the sort plane to zero. object_index/widget_index are repurposed as the
 * group's first_triangle_index/triangle_count. No-op (silently) if render_proc is null; warns once (via
 * warned_2) if the transparent geometry group pool is exhausted. */

#include "headers/transparent_geometry_group.h"
#include "headers/rasterizer_window_begin_parameters.h"
#include "headers/blam_data_globals.h"


extern transparent_geometry_group *rasterizer_transparent_geometry_new_group(void);

void _rasterizer_widget_submit(int object_index, int widget_index, const real_point3d *centroid,
    void (*render_proc)(int, int))
{
    if ( !render_proc )
        return;

    transparent_geometry_group *group = rasterizer_transparent_geometry_new_group();
    if ( !group )
    {
        if ( !warned_2 )
            warned_2 = 1;
        return;
    }

    group->geometry_flags = 0;
    group->object_index = 0;
    group->source_object_index = 0;
    group->shader = nullptr;
    group->shader_permutation_index = 0;
    group->effect.type = 0;
    group->dynamic_triangle_buffer_index = -1;
    group->render_proc = render_proc;  /* union arm — widget path is tagged by group->shader == NULL */
    group->first_triangle_index = object_index;
    group->triangle_count = widget_index;
    group->dynamic_vertex_buffer_index = -1;
    group->vertex_buffers = nullptr;
    group->lightmap = nullptr;

    group->z_sort = -(global_window_parameters.camera.forward.n[1] * (centroid->n[1] - global_window_parameters.camera.position.n[1])
                     + global_window_parameters.camera.forward.n[0] * (centroid->n[0] - global_window_parameters.camera.position.n[0])
                     + global_window_parameters.camera.forward.n[2] * (centroid->n[2] - global_window_parameters.camera.position.n[2]));

    group->centroid = *centroid;
    group->model_base_map_scale.n[1] = 1.0f;
    group->model_base_map_scale.n[0] = 1.0f;
    group->plane.n.n[0] = 0.0f;
    group->plane.n.n[1] = 0.0f;
    group->plane.n.n[2] = 0.0f;
    group->plane.d = 0.0f;
    group->prev_group_presorted_index = -1;
    group->next_group_presorted_index = -1;
    group->active_camouflage_transparent_source_object_index = 0;
    group->cortana_hack = 0;
    group->node_matrices = nullptr;
    group->node_matrix_count = 0;
    group->lighting = nullptr;
    group->animation = nullptr;
}
