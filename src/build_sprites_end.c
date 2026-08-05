/* build_sprites_end @0x837EC390 — finalize a sprite batch: average the accumulated centroid into world space,
 * then flush each group's vertex buffer as either screen-space or unlit world geometry. Simplified the
 * decompiler's fcfid int->double conversion dance (HIDWORD/LODWORD union trick) to the plain
 * count ? 1.0f/count : 0.0f it computes. */

#include <stdint.h>
#include "headers/build_sprite_data.h"
#include "headers/build_sprite_flags.h"
#include "headers/build_sprites_internal_flags.h"
#include "headers/render_globals.h"
#include "headers/blam_data_globals.h"


#include "headers/rasterizer_dynamic_screen_geometry_parameters.h"
#include "headers/bitmap_data.h"
#include "headers/render_animation.h"
#include "headers/real_point3d.h"
extern real_point3d *matrix4x3_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);
extern void rasterizer_dynamic_vertices_unlock(int dynamic_vertex_buffer_index);
extern void rasterizer_dynamic_vertices_delete(int dynamic_vertex_buffer_index);
extern void rasterizer_dynamic_screen_geometry_draw(const rasterizer_dynamic_screen_geometry_parameters *parameters, int dynamic_triangle_buffer_index, int dynamic_vertex_buffer_index, int triangle_count);
extern void rasterizer_dynamic_unlit_geometry_draw(const shader *shader, const bitmap_data *primary_map, const render_animation *animation, int dynamic_triangle_buffer_index, int dynamic_vertex_buffer_index, int triangle_count, const real_point3d *centroid, unsigned int geometry_flags);

void build_sprites_end(build_sprite_data *data)
{
    float oo_sprite_count = data->sprite_count ? 1.0f / (float)data->sprite_count : 0.0f;

    data->centroid.n[0] *= oo_sprite_count;
    data->centroid.n[1] *= oo_sprite_count;
    data->centroid.n[2] *= oo_sprite_count;
    matrix4x3_transform_point(&render.frustum.view_to_world, &data->centroid, &data->centroid);

    for ( int16_t group_index = 0; group_index < data->group_count; group_index++ )
    {
        build_sprite_group *group = &data->groups[group_index];

        if ( group->vertex_buffer_index != -1 )
            rasterizer_dynamic_vertices_unlock(group->vertex_buffer_index);

        int16_t triangle_count = group->sprite_count;
        if ( triangle_count )
        {
            if ( (data->flags & (1u << _build_sprite_viewer_space_bit)) != 0 )
            {
                rasterizer_dynamic_screen_geometry_draw(0, -4, group->vertex_buffer_index, 2 * triangle_count);
            }
            else
            {
                unsigned int geometry_flags = ((data->flags << 6) & 0x80) | 0x20;
                rasterizer_dynamic_unlit_geometry_draw((const shader *)data->shader, group->bitmap, 0, -4,
                                                        group->vertex_buffer_index, 2 * triangle_count,
                                                        &data->centroid, geometry_flags);
            }
        }

        if ( group->vertex_buffer_index != -1 )
            rasterizer_dynamic_vertices_delete(group->vertex_buffer_index);
    }

    /* clear the internal build_sprites "valid/active" marker (set in build_sprites_begin) — enum
     * $A2AAABCE (_build_sprites_valid_bit=2), distinct from the build_sprite_flags used for bit 0 above. */
    data->flags &= ~(1u << _build_sprites_valid_bit);
}
