/* antenna_render_proper @0x8380D778 — render one antenna's (springy aerial) vertex chain as a strip of
 * sprites. Computes a distance-falloff alpha scale (0 at cutoff_pixels-or-closer, 1 at falloff_pixels-or-
 * beyond, linear between), then for each definition vertex builds a sprite oriented along the segment
 * direction (current simulated vertex position minus the previous one), tinted/faded per the definition
 * vertex's stored color and the runtime alpha, skipping fully-transparent or fully-opaque-scale-zero
 * segments.
 *
 * Antenna vertex state is the antenna_datum.vertices[] array (antenna_vertex_datum, 32-byte stride).
 * Definition vertex records are the antenna_vertex DB struct (128-byte stride within
 * antenna_definition.vertices). */

#include <stdint.h>
#include "headers/antenna_datum.h"
#include "headers/antenna_vertex_datum.h"
#include "headers/antenna_definition.h"
#include "headers/antenna_vertex.h"
#include "headers/real_vector3d.h"
#include "headers/real_argb_color.h"
#include "headers/build_sprite_data.h"
#include "headers/shader_effect.h"
#include "headers/blam_data_globals.h"


#include "headers/shader_effect.h"
#include "headers/real_point3d.h"
extern void build_sprites_begin(build_sprite_data *data, int16_t maximum_sprite_count, int bitmap_group_index, const shader_effect *shader, unsigned int flags);
extern void build_sprite(build_sprite_data *data, int16_t mode, int16_t sequence_index, int16_t sprite_index, const real_point3d *untransformed_origin, const real_vector3d *untransformed_direction, float rotation, float scale, const real_argb_color *color, float fade, unsigned int flags);
extern void build_sprites_end(build_sprite_data *data);

void antenna_render_proper(antenna_datum *antenna, antenna_definition *antenna_definition)
{
    int count = antenna_definition->vertices.count;
    if ( count == 0 )
        return;

    float falloff_scale = (100.0f - antenna_definition->cutoff_pixels)
        / (antenna_definition->falloff_pixels - antenna_definition->cutoff_pixels);
    if ( falloff_scale < 0.0f )
        falloff_scale = 0.0f;
    else if ( falloff_scale > 1.0f )
        falloff_scale = 1.0f;

    build_sprite_data sprites;
    build_sprites_begin(&sprites, count, antenna_definition->texture.index, &global_shader_effect_alpha_blended, 0);

    for ( int16_t i = 0; i < antenna_definition->vertices.count; i = (int16_t)(i + 1) )
    {
        /* recovered: (char*)antenna + 32*i -> &antenna->vertices[i]; float idx +15/16/17 = vertices[i+1].p */
        antenna_vertex_datum *vertex = &antenna->vertices[i];
        antenna_vertex_datum *next_vertex = &antenna->vertices[i + 1];
        antenna_vertex *definition_vertex = &((antenna_vertex *)antenna_definition->vertices.address)[i];

        real_vector3d direction;
        direction.n[0] = next_vertex->p.n[0] - vertex->p.n[0];
        direction.n[1] = next_vertex->p.n[1] - vertex->p.n[1];
        direction.n[2] = next_vertex->p.n[2] - vertex->p.n[2];

        /* DEVIATION: full 16-byte struct copy (lfs + fused ld/std pair over red+green); was a
         * mixed float / uint64 word-pun copy of the same four members */
        real_argb_color color = definition_vertex->color;

        float vertex_scale = vertex->sprite_scale;

        if ( vertex_scale != 0.0f && falloff_scale > 0.0f )
        {
            build_sprite(&sprites, 1, definition_vertex->sequence_index, 0,
                &vertex->p, &direction, 0.0f, vertex_scale, &color, falloff_scale,
                (unsigned int)&color);
        }
    }

    build_sprites_end(&sprites);
}
