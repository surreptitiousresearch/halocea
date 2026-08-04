/* lightning_submit @0x8380A0C0 — render one object's lightning ('elec') widget: a bundle of animated
 * electric-arc bolts. For each of the definition's `count` overlaid instances it walks the definition's
 * control markers (resolved by name on the emitting object). Consecutive markers form a bolt; the path
 * between two markers is recursively subdivided (octaves_to_next_marker levels of midpoint displacement,
 * each midpoint jittered along the camera-facing side vector by a random, amplitude-tapered offset). At a
 * marker flagged as the bolt end (or the last marker) the accumulated intermediate points are billboarded
 * into a two-vertex-per-node triangle strip (camera-facing, per-node width and ARGB tint), an AABB centroid
 * is computed, and the strip is submitted through rasterizer_dynamic_unlit_geometry_draw.
 *
 * Reconstructed from a reg-alloc-confused decompile (fabricated ~60-arg signature; DB proto = 4 args). The
 * ~128 KB stack frame (0x20280) is dominated by the intermediate_lightning_point scratch array and the
 * resolved object_marker; the decompiler split the single object_marker into a marker buffer and a separate
 * `matrix` (its .matrix field at +0x38, confirmed via disasm 0x8380A298). Per-node cross products against
 * the camera forward are written here in direct cross form (the decompiler emitted them in register-schedule
 * order). The `lighting` parameter is unused on this path (r5 is never read). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/lightning_marker_flags.h"
#include "headers/lightning_globals.h"
#include "headers/lightning_datum.h"
#include "headers/lightning_definition.h"
#include "headers/lightning_marker_definition.h"
#include "headers/intermediate_lightning_point.h"
#include "headers/global_tag_instances.h"
#include "headers/object_marker.h"
#include "headers/render_animation.h"
#include "headers/render_lighting.h"
#include "headers/render_globals.h"
#include "headers/bitmap_data.h"
#include "headers/bitmap_group.h"
#include "headers/shader.h"
#include "headers/shader_effect.h"
#include "headers/rasterizer_globals_struct.h"
#include "headers/rasterizer_lock_operation.h"
#include "headers/rasterizer_vertex_type.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_rgb_color.h"
#include "headers/real_matrix4x3.h"
#include "headers/ppc_intrinsics.h"
#include "headers/blam_data_globals.h"

/* Fixed stack scratch for the bolt path. The real frame is 0x20280 bytes, dominated by this array; the exact
 * element bound is not recoverable from the decompile (it is capped by the tag's marker count and octaves),
 * so it is sized to the observed frame. */
#define LIGHTNING_MAXIMUM_INTERMEDIATE_POINTS 4096

#include "headers/real_matrix4x3.h"
extern lightning_globals_type lightning_globals;

extern void *datum_get(data_array *data, int index);
extern int16_t object_get_marker_by_name(int object_index, const char *name, object_marker *markers, int16_t maximum_marker_count);
extern int _texture_cache_bitmap_get_hardware_format(bitmap_data *bitmap, uint8_t block, uint8_t load);
extern void lightning_offset_marker_position(real_point3d *position, const real_matrix4x3 *matrix, const real_vector3d *random_position_bounds);
extern float normalize3d(real_vector3d *v);
extern uint32_t *get_global_local_random_seed_address(void);
extern float real_seed_random(uint32_t *seed);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);
extern int rasterizer_dynamic_vertices_new(int16_t type, int count);
extern void * rasterizer_dynamic_vertices_lock(int dynamic_vertex_buffer_index);
extern void rasterizer_dynamic_vertices_unlock(int dynamic_vertex_buffer_index);
extern void rasterizer_dynamic_vertices_delete(int dynamic_vertex_buffer_index);
extern void rasterizer_dynamic_unlit_geometry_draw(const shader *shader, const bitmap_data *primary_map, const render_animation *animation, int dynamic_triangle_buffer_index, int dynamic_vertex_buffer_index, int triangle_count, const real_point3d *centroid, unsigned int geometry_flags);

/* animation function look-up: source index is 1..4 into the animation's value or color channel */
static float lightning_animation_value(const render_animation *animation, __int16 source, float fallback)
{
    if ( animation && animation->values && source >= 1 && source <= 4 )
        return animation->values[source - 1];
    return fallback;
}

void lightning_submit(int object_index, int widget_index, const render_lighting *lighting,
                      const render_animation *animation)
{
    (void)lighting; /* unused on this path (r5 never read) */

    if ( object_index == -1 || widget_index == -1 )
        return;

    lightning_datum *datum = datum_get(lightning_globals.lightning_data, widget_index);
    lightning_definition *definition = TAG_GET(lightning_definition, datum->definition_index);

    if ( definition->markers.count <= 0 )
        return;

    const lightning_marker_definition *markers = (const lightning_marker_definition *)definition->markers.address;
    object_marker marker;
    if ( object_get_marker_by_name(object_index, markers[0].attachment_marker, &marker, 1) <= 0 )
        return;

    const bitmap_data *bitmap =
        (const bitmap_data *)(TAG_GET(bitmap_group, definition->map.index))->bitmaps.address;   /* +100 */
    if ( !_texture_cache_bitmap_get_hardware_format((bitmap_data *)bitmap, 0, 1u) )
        return;

    if ( definition->count <= 0 )
        return;

    rasterizer_globals_struct *rasterizer = &rasterizer_globals;
    intermediate_lightning_point points[LIGHTNING_MAXIMUM_INTERMEDIATE_POINTS];

    const float camera_forward_x = render.camera.forward.n[0];
    const float camera_forward_y = render.camera.forward.n[1];
    const float camera_forward_z = render.camera.forward.n[2];

    int instance = 0;
    do
    {
        __int16 point_count = 0;
        char first_marker = 1;
        float jitter_scale = lightning_animation_value(animation, definition->jitter_scale_source, 1.0f);

        int marker_index = 0;
        do
        {
            const lightning_marker_definition *marker_def = &markers[marker_index];

            if ( first_marker )
            {
                point_count = 0;
                object_get_marker_by_name(object_index, marker_def->attachment_marker, &marker, 1);
                points[0].position = marker.matrix.position;
                lightning_offset_marker_position(&points[0].position, &marker.matrix,
                                                 &marker_def->random_position_bounds);
                points[0].width = marker_def->thickness;
                points[0].color = marker_def->tint;
                first_marker = 0;
            }

            if ( (marker_def->flags & (1u << _lightning_marker_not_connected_to_next_marker_bit)) != 0 || marker_index == definition->markers.count - 1 )
            {
                /* end of a bolt: billboard the accumulated points into a camera-facing strip and submit */
                rasterizer->current_lock_operation = _rasterizer_lock_lightning;
                if ( point_count > 2 )
                {
                    int point_total = (__int16)(point_count + 1);
                    int vertex_count = 2 * point_total;
                    int buffer = rasterizer_dynamic_vertices_new(_rasterizer_vertex_type_dynamic_unlit, vertex_count);
                    if ( buffer != -1 )
                    {
                        float *vertex = rasterizer_dynamic_vertices_lock(buffer);
                        float inverse_total = (float)1.0 / (float)point_total;
                        float uv_base = real_seed_random(get_global_local_random_seed_address());
                        float thickness_scale =
                            lightning_animation_value(animation, definition->thickness_scale_source, 1.0f);
                        float brightness_scale =
                            lightning_animation_value(animation, definition->brightness_scale_source, 1.0f);
                        const real_rgb_color *tint = global_real_rgb_white;
                        if ( animation && animation->colors && definition->tint_modulation_source >= 1
                          && definition->tint_modulation_source <= 4 )
                            tint = &animation->colors[definition->tint_modulation_source - 1];

                        float aabb_min_x = 0.0, aabb_min_y = 0.0, aabb_min_z = 0.0;
                        float aabb_max_x = 0.0, aabb_max_y = 0.0, aabb_max_z = 0.0;

                        for ( int i = 0; i < point_total; i = (__int16)(i + 1) )
                        {
                            intermediate_lightning_point *point = &points[i];
                            /* recovered: point - 8 -> &points[i - 1] (8 floats == sizeof intermediate_lightning_point) */
                            const intermediate_lightning_point *previous = (i <= 0) ? point : &points[i - 1];
                            const intermediate_lightning_point *next = (i >= point_total - 1) ? point : &points[i + 1];

                            /* camera-facing side vector = normalize(cross(segment_direction, camera_forward)) */
                            float delta_x = next->position.n[0] - previous->position.n[0];
                            float delta_y = next->position.n[1] - previous->position.n[1];
                            float delta_z = next->position.n[2] - previous->position.n[2];
                            real_vector3d side;
                            side.n[0] = ((delta_y * camera_forward_z)
                                              - (delta_z * camera_forward_y));
                            side.n[1] = ((delta_z * camera_forward_x)
                                              - (delta_x * camera_forward_z));
                            side.n[2] = ((delta_x * camera_forward_y)
                                              - (delta_y * camera_forward_x));
                            float length_squared = ((side.n[2] * side.n[2])
                                                         + ((side.n[1] * side.n[1])
                                                                 + (side.n[0] * side.n[0])));
                            if ( length_squared != 0.0 )
                            {
                                float inverse_length = ((float)1.0 / __fsqrts(length_squared));
                                side.n[0] = (inverse_length * side.n[0]);
                                side.n[1] = (inverse_length * side.n[1]);
                                side.n[2] = (inverse_length * side.n[2]);
                            }

                            float half_width = (point->width * thickness_scale);
                            float uv_u = (((float)i * inverse_total) + uv_base);

                            /* pack the per-node ARGB tint modulated by animation tint/brightness */
                            unsigned __int8 a = (unsigned __int8)(int)((point->color.alpha * brightness_scale) * (float)255.0);
                            unsigned __int8 r = (unsigned __int8)(int)((point->color.rgb.red * tint->n[0]) * (float)255.0);
                            unsigned __int8 g = (unsigned __int8)(int)((point->color.rgb.green * tint->n[1]) * (float)255.0);
                            unsigned __int8 b = (unsigned __int8)(int)((point->color.rgb.blue * tint->n[2]) * (float)255.0);
                            int packed_color = (a << 24) | (r << 16) | (g << 8) | b;

                            /* two vertices: node position offset ±half_width along the side vector */
                            vertex[0] = ((side.n[0] * half_width) + point->position.n[0]);
                            vertex[1] = ((side.n[1] * half_width) + point->position.n[1]);
                            vertex[2] = ((side.n[2] * half_width) + point->position.n[2]);
                            *(int *)&vertex[3] = packed_color;
                            vertex[4] = uv_u;
                            vertex[5] = 0.0;
                            vertex[6] = ((side.n[0] * -half_width) + point->position.n[0]);
                            vertex[7] = ((side.n[1] * -half_width) + point->position.n[1]);
                            vertex[8] = ((side.n[2] * -half_width) + point->position.n[2]);
                            *(int *)&vertex[9] = packed_color;
                            vertex[10] = uv_u;
                            vertex[11] = 1.0;
                            vertex += 12;

                            if ( i )
                            {
                                if ( point->position.n[0] <= aabb_min_x ) aabb_min_x = point->position.n[0];
                                if ( point->position.n[1] <= aabb_min_y ) aabb_min_y = point->position.n[1];
                                if ( point->position.n[2] <= aabb_min_z ) aabb_min_z = point->position.n[2];
                                if ( point->position.n[0] > aabb_max_x )  aabb_max_x = point->position.n[0];
                                if ( point->position.n[1] > aabb_max_y )  aabb_max_y = point->position.n[1];
                                if ( point->position.n[2] > aabb_max_z )  aabb_max_z = point->position.n[2];
                            }
                            else
                            {
                                aabb_max_x = point->position.n[0]; aabb_max_y = point->position.n[1]; aabb_max_z = point->position.n[2];
                                aabb_min_x = point->position.n[0]; aabb_min_y = point->position.n[1]; aabb_min_z = point->position.n[2];
                            }
                        }

                        real_point3d centroid;
                        centroid.n[0] = ((aabb_min_x + aabb_max_x) * (float)0.5);
                        centroid.n[1] = ((aabb_min_y + aabb_max_y) * (float)0.5);
                        centroid.n[2] = ((aabb_min_z + aabb_max_z) * (float)0.5);

                        const shader *strip_shader = (definition->shaders.count <= 0)
                            ? (const shader *)&global_shader_effect_additive
                            : (const shader *)definition->shaders.address;

                        rasterizer_dynamic_vertices_unlock(buffer);
                        rasterizer_dynamic_unlit_geometry_draw(strip_shader, bitmap, animation,
                                                               -2 * point_total, buffer, vertex_count - 2,
                                                               &centroid, 0);
                        rasterizer_dynamic_vertices_delete(buffer);
                    }
                    first_marker = 1;
                }
                rasterizer->current_lock_operation = _rasterizer_lock_none;
            }
            else
            {
                /* interior marker: recursively subdivide the path toward the next marker */
                __int16 octaves = marker_def->octaves_to_next_marker;
                float level_scale = 1.0f;
                const lightning_marker_definition *next_marker_def = &markers[marker_index + 1];
                object_get_marker_by_name(object_index, next_marker_def->attachment_marker, &marker, 1);
                int segment_count = (__int16)(1 << octaves);
                int base_index = point_count;

                /* seed the next marker's endpoint at base_index + segment_count */
                intermediate_lightning_point *end = &points[base_index + segment_count];
                end->position = marker.matrix.position;
                lightning_offset_marker_position(&end->position, &marker.matrix,
                                                 &next_marker_def->random_position_bounds);
                end->width = next_marker_def->thickness;
                end->color = next_marker_def->tint;

                /* side vector for this segment = normalize(cross(segment_delta, camera_forward)) */
                const intermediate_lightning_point *start_point = &points[base_index];
                const intermediate_lightning_point *end_point = end;
                float delta_x = end_point->position.n[0] - start_point->position.n[0];
                float delta_y = end_point->position.n[1] - start_point->position.n[1];
                float delta_z = end_point->position.n[2] - start_point->position.n[2];
                real_vector3d side;
                side.n[0] = ((delta_y * camera_forward_z) - (delta_z * camera_forward_y));
                side.n[1] = ((delta_z * camera_forward_x) - (delta_x * camera_forward_z));
                side.n[2] = ((delta_x * camera_forward_y) - (delta_y * camera_forward_x));
                if ( normalize3d(&side) == 0.0 )
                    side = *(const real_vector3d *)global_z_axis3d; /* global_z_axis3d is real_point3d*; same 12-byte layout */

                if ( octaves >= 1 )
                {
                    float side_x = side.n[0];
                    float side_y = side.n[1];
                    float side_z = side.n[2];
                    int level = 1;
                    do
                    {
                        int half_span = (__int16)(1 << (octaves - level));
                        if ( half_span < segment_count )
                        {
                            float segment_count_f = (float)segment_count;
                            int index = half_span;
                            do
                            {
                                const intermediate_lightning_point *left = &points[base_index + index - half_span];
                                const intermediate_lightning_point *right = &points[base_index + index + half_span];
                                intermediate_lightning_point *middle = &points[base_index + index];
                                /* jitter amplitude tapers between the two markers' random_jitter_offset,
                                 * halved per subdivision level and scaled by the instance jitter scale */
                                float amplitude =
                                    ((((((float)index / segment_count_f)
                                            * (next_marker_def->random_jitter_offset
                                                    - marker_def->random_jitter_offset))
                                        + marker_def->random_jitter_offset)
                                    * level_scale)
                                    * jitter_scale);
                                float displacement =
                                    (real_seed_random_range(get_global_local_random_seed_address(),
                                                                   -1.0, 1.0) * amplitude);
                                middle->position.n[0] = (((right->position.n[0] + left->position.n[0]) * (float)0.5)
                                                  + (displacement * side_x));
                                middle->position.n[1] = (((right->position.n[1] + left->position.n[1]) * (float)0.5)
                                                  + (displacement * side_y));
                                middle->position.n[2] = (((right->position.n[2] + left->position.n[2]) * (float)0.5)
                                                  + (displacement * side_z));
                                middle->width    = ((right->width + left->width) * (float)0.5);
                                middle->color.n[0] = ((right->color.n[0] + left->color.n[0]) * (float)0.5);
                                middle->color.n[1] = ((right->color.n[1] + left->color.n[1]) * (float)0.5);
                                middle->color.n[2] = ((right->color.n[2] + left->color.n[2]) * (float)0.5);
                                middle->color.n[3] = ((right->color.n[3] + left->color.n[3]) * (float)0.5);
                                index = (__int16)(2 * half_span + index);
                            }
                            while ( index < segment_count );
                        }
                        level_scale = (level_scale * (float)0.5);
                        level = (__int16)(level + 1);
                    }
                    while ( level <= octaves );
                }

                point_count = (__int16)(segment_count + base_index);
            }

            marker_index = (__int16)(marker_index + 1);
        }
        while ( marker_index < definition->markers.count );

        instance = (__int16)(instance + 1);
    }
    while ( instance < definition->count );
}
