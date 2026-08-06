/* build_sprites_end @0x837EC390 — finalize a sprite batch: average the accumulated centroid into world space,
 * then flush each group's vertex buffer as either screen-space or unlit world geometry. Simplified the
 * decompiler's fcfid int->double conversion dance (HIDWORD/LODWORD union trick) to the plain
 * count ? 1.0f/count : 0.0f it computes.
 *
 * DEVIATION: data->flags was previously read through build_sprite_flags ($D9CCB6DD, the per-sprite
 * u/v-mirror word that build_sprite() takes as an argument). The word build_sprites_begin stores into
 * build_sprite_data.flags is the sibling enum build_sprites_flags ($1DEC01DC: bit 0 screen_space,
 * bit 1 first_person), which dovetails with build_sprites_internal_flags ($A2AAABCE) starting at bit 2
 * — build_sprite_flags would collide there. Corroborated by render_particles.c passing
 * `attached_to_first_person_weapon ? 2 : 0` into build_sprites_begin. Same bit-0 value either way, so
 * this is a naming correction, not a behaviour change. */

#include <stdint.h>
#include "headers/build_sprite_data.h"
#include "headers/build_sprites_flags.h"
#include "headers/build_sprites_internal_flags.h"
#include "headers/rasterizer_geometry_flags.h"
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
            /* a negative dynamic_triangle_buffer_index is not a buffer handle: the transparent-group draw
             * path negates it back into vertices-per-primitive (see
             * rasterizer_transparent_geometry_group_draw_internal), so -4 selects the 4-vertices-per-
             * primitive quad form each sprite is built as. Left raw per arg_catalog.tsv, which already
             * adjudicates this slot for both callees as "leave". (The screen-geometry callee is a no-op
             * stub in this build, so its copy of the value is never decoded.) */
            if ( (data->flags & (1u << _build_sprites_screen_space_bit)) != 0 )
            {
                rasterizer_dynamic_screen_geometry_draw(0, -4, group->vertex_buffer_index, 2 * triangle_count);
            }
            else
            {
                /* relocate the batch's first-person bit into the rasterizer's: `rlwinm r11, r11, 6,24,24`
                 * @0x837EC480 then `ori r10, r11, 0x20` @0x837EC490. Sprite vertices are built in view
                 * space (the centroid above is transformed view->world), hence the viewspace bit. */
                unsigned int geometry_flags =
                        ((data->flags << (_rasterizer_geometry_first_person_bit - _build_sprites_first_person_bit))
                                & (1u << _rasterizer_geometry_first_person_bit))
                        | (1u << _rasterizer_geometry_viewspace_bit);
                rasterizer_dynamic_unlit_geometry_draw((const shader *)data->shader, group->bitmap, 0, -4,
                                                        group->vertex_buffer_index, 2 * triangle_count,
                                                        &data->centroid, geometry_flags);
            }
        }

        if ( group->vertex_buffer_index != -1 )
            rasterizer_dynamic_vertices_delete(group->vertex_buffer_index);
    }

    /* clear the internal build_sprites "valid/active" marker (set in build_sprites_begin) — enum
     * $A2AAABCE (_build_sprites_valid_bit=2), the bit immediately above the build_sprites_flags pair. */
    data->flags &= ~(1u << _build_sprites_valid_bit);
}
