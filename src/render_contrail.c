/* render_contrail @0x837EA8F8 — build and submit the triangle-strip geometry for one contrail ribbon.
 *
 * A contrail is a chain of "points" (contrail_point_data, 56-byte records linked by a next index) laid
 * down behind a moving object/marker. Each point references a contrail_state (104-byte tag block element)
 * that supplies its width and a two-key colour ramp; adjacent states are cross-faded by the point's
 * transition parameter. This walks the point list for `instance_index`, emitting two vertices (a ribbon
 * cross-section) per point into a locked dynamic vertex buffer, wiring up the strip's triangle indices,
 * then submits the whole ribbon via rasterizer_dynamic_unlit_geometry_draw.
 *
 * The ribbon cross-section orientation depends on contrail_definition->render_type
 * ($A95FA4AC... _contrail_render_type_*):
 *   0 vertical              — offset along world Z (vertical ribbon)
 *   1 horizontal / 2 media  — offset along the screen-space (x,y) tangent to the neighbouring point
 *   3 ground                — not rendered
 *   4 viewer                — camera-facing: cross-product of the segment direction with the eye vector
 *
 * Vertex layout: 6 floats each — {x, y, z, packed_ARGB(as int), u, v}; two vertices (12 floats) per point.
 *
 * contrail_point (56B) fields used (word index N = byte 4*N):
 *   +0x02 byte flags (bit1 = has state transition), +0x03 byte state index,
 *   [1] transition t, [3] age t, [7..9] position xyz, [13] next-point index (-1 = end).
 * contrail_state (104B) fields used:
 *   [16] width key, [17..20] colour key0 (A,R,G,B), [21..24] colour key1, [25] scale_flags
 *   (_contrail_state_width_bit=4 → scale width by density, _contrail_state_color_bit=5 → scale colour-t
 *   by density); word indices >=42 read the transition
 *   target state (the following 104-byte element).
 *
 * DEVIATIONS from the decompiler:
 *   - The packed vertex colour was emitted with __ROL4__; it is exactly ARGB = A<<24|R<<16|G<<8|B and
 *     written directly here.
 *   - shader.effect.framebuffer_fade_mode is read at shader+0x2C (the contrail shader extends past the
 *     40-byte base modelled in the header).
 *   - The decompiler `break`s out of the point loop for render_type 4 then jumps back into the emit code;
 *     that is a structured inline branch (this iteration's camera-facing geometry), reproduced here as an
 *     ordinary branch so all four render types share the emit/advance tail. */

#include <stdint.h>
#include "headers/contrail_datum.h"
#include "headers/contrail_definition.h"
#include "headers/contrail_point_datum.h"
#include "headers/contrail_point_datum_flags.h"
#include "headers/contrail_point_state.h"
#include "headers/bitmap_data.h"
#include "headers/rasterizer_triangle.h"
#include "headers/rasterizer_globals.h"
#include "headers/shader.h"
#include "headers/render_globals.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/_object_definition.h"
#include "headers/object_attachment_definition.h"
#include <math.h>
#include "headers/contrail_definition_flags.h"
#include "headers/contrail_render_type.h"
#include "headers/rasterizer_lock_operation.h"
#include "headers/contrail_definition_scaleable_values_flags.h"
#include "headers/contrail_point_state_scaleable_values_flags.h"
#include "headers/object_datum.h"
#include "headers/real_rgb_color.h"
#include "headers/blam_data_globals.h"
#include "headers/rasterizer_vertex_type.h"



#include "headers/render_animation.h"
extern bitmap_data *bitmap_group_get_bitmap_from_sequence(int bitmap_group_index, int16_t sequence_index, int16_t frame_index);
extern int _texture_cache_bitmap_get_hardware_format(bitmap_data *bitmap, uint8_t block, uint8_t load);
extern int rasterizer_dynamic_triangles_new(int count);
extern int rasterizer_dynamic_vertices_new(int16_t type, int count);
extern rasterizer_triangle *rasterizer_dynamic_triangles_lock(int dynamic_triangle_buffer_index);
extern void * rasterizer_dynamic_vertices_lock(int dynamic_vertex_buffer_index);
extern void rasterizer_dynamic_triangles_unlock(int dynamic_triangle_buffer_index);
extern void rasterizer_dynamic_vertices_unlock(int dynamic_vertex_buffer_index);
extern void rasterizer_dynamic_triangles_delete(int dynamic_triangle_buffer_index);
extern void rasterizer_dynamic_vertices_delete(int dynamic_vertex_buffer_index);
extern void rasterizer_dynamic_unlit_geometry_draw(const shader *shader, const bitmap_data *primary_map, const render_animation *animation, int dynamic_triangle_buffer_index, int dynamic_vertex_buffer_index, int triangle_count, const real_point3d *centroid, unsigned int geometry_flags);
extern float contrail_fade(contrail_definition *contrail_definition, int16_t fade_mode, const real_point3d *world_point, const real_vector3d *world_normal);
extern float normalize3d(real_vector3d *v);

void render_contrail(contrail_datum *contrail, contrail_definition *contrail_definition, int16_t instance_index)
{
    const bitmap_data *bitmap =
        bitmap_group_get_bitmap_from_sequence(contrail_definition->bitmap.index, contrail->sequence_index, contrail->frame_index);

    rasterizer_globals.current_lock_operation = _rasterizer_lock_contrail;
    /* lazy hardware-format init mutates the bitmap (rasterizer_bitmap_new); drop the const view */
    if ( !_texture_cache_bitmap_get_hardware_format((bitmap_data *)bitmap, 0, 1u) )
    {
        rasterizer_globals.current_lock_operation = _rasterizer_lock_none;
        return;
    }

    int segment_count = (int16_t)(contrail->contrail_point_counts[instance_index] - 1);
    int vertex_count = (int16_t)(2 * segment_count + 2);
    int triangle_handle = rasterizer_dynamic_triangles_new((int16_t)(2 * segment_count));
    int vertex_handle = rasterizer_dynamic_vertices_new(_rasterizer_vertex_type_dynamic_unlit, vertex_count);

    if ( triangle_handle != -1 && vertex_handle != -1 )
    {
        rasterizer_triangle *triangle = rasterizer_dynamic_triangles_lock(triangle_handle);
        float *vertex = rasterizer_dynamic_vertices_lock(vertex_handle);

        float texture_offset_u = contrail->texture_offset_u;
        uint16_t scale_flags = contrail_definition->scale_flags;
        float texture_repeats_u = contrail_definition->texture_repeats_u;
        int16_t framebuffer_fade_mode = contrail_definition->shader.effect.framebuffer_fade_mode;
        int apply_orientation = framebuffer_fade_mode != 0;

        real_point3d average_position;
        average_position.n[0] = global_origin3d->n[0];
        average_position.n[1] = global_origin3d->n[1];
        average_position.n[2] = global_origin3d->n[2];

        if ( (scale_flags & (1u << _contrail_texture_repeats_u_bit)) != 0 )
            texture_repeats_u = contrail->density * texture_repeats_u;

        float texture_repeats_v = contrail_definition->texture_repeats_v;
        if ( (scale_flags & (1u << _contrail_texture_repeats_v_bit)) != 0 )
            texture_repeats_v = contrail->density * contrail_definition->texture_repeats_v;

        float texture_u_step = -texture_repeats_u;
        float texture_v_far = texture_repeats_v + contrail->texture_offset_v;

        contrail_point_datum *previous_point = 0;
        int point_index = contrail->first_contrail_point_indices[instance_index];

        if ( point_index != -1 )
        {
            while ( 1 )
            {
                contrail_point_datum *point = (contrail_point_datum *)contrail_point_data->data + (uint16_t)point_index;
                contrail_point_state *state = (contrail_point_state *)contrail_definition->states.address + point->state_index;
                int state_flags = state->scale_flags;

                float colour_t = 1.0f;
                if ( (state_flags & (1u << _contrail_state_color_bit)) != 0 )
                    colour_t = point->density;
                float width = state->width;
                if ( (state_flags & (1u << _contrail_state_width_bit)) != 0 )
                    width = point->density * state->width;

                float colour_a = (state->color_upper_bound.n[0] - state->color_lower_bound.n[0]) * colour_t + state->color_lower_bound.n[0];
                float colour_r = (state->color_upper_bound.n[1] - state->color_lower_bound.n[1]) * colour_t + state->color_lower_bound.n[1];
                float colour_g = (state->color_upper_bound.n[2] - state->color_lower_bound.n[2]) * colour_t + state->color_lower_bound.n[2];
                float colour_b = (state->color_upper_bound.n[3] - state->color_lower_bound.n[3]) * colour_t + state->color_lower_bound.n[3];

                /* cross-fade toward the following state when the point carries a transition */
                if ( (point->flags & (1u << _contrail_point_transitioning_bit)) != 0 )
                {
                    /* state[1] = the transition-target state (following 104-byte element) */
                    int next_state_flags = state[1].scale_flags;
                    float next_colour_t = 1.0f;
                    if ( (next_state_flags & (1u << _contrail_state_color_bit)) != 0 )
                        next_colour_t = point->density;
                    float next_width = state[1].width;
                    if ( (next_state_flags & (1u << _contrail_state_width_bit)) != 0 )
                        next_width = point->density * state[1].width;

                    float transition_t = point->time;
                    width = (next_width - width) * transition_t + width;
                    colour_a = ((((state[1].color_upper_bound.n[0] - state[1].color_lower_bound.n[0]) * next_colour_t + state[1].color_lower_bound.n[0]) - colour_a) * transition_t) + colour_a;
                    colour_r = ((((state[1].color_upper_bound.n[1] - state[1].color_lower_bound.n[1]) * next_colour_t + state[1].color_lower_bound.n[1]) - colour_r) * transition_t) + colour_r;
                    colour_g = ((((state[1].color_upper_bound.n[2] - state[1].color_lower_bound.n[2]) * next_colour_t + state[1].color_lower_bound.n[2]) - colour_g) * transition_t) + colour_g;
                    colour_b = ((((state[1].color_upper_bound.n[3] - state[1].color_lower_bound.n[3]) * next_colour_t + state[1].color_lower_bound.n[3]) - colour_b) * transition_t) + colour_b;
                }

                /* modulate colour by the attached object's marker-node tint, if any */
                if ( contrail->object_index != -1 )
                {
                    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, contrail->object_index)->datum;
                    _object_definition *object_def = TAG_GET(_object_definition, object->definition_index);
                    /* the attachment's change_color_reference (DB member @+52) is consumed here as a
                     * 1-based node index into the object's runtime node array (0 = none). */
                    object_attachment_definition *attachment =
                        &((object_attachment_definition *)object_def->attachments.address)[contrail->attachment_index];
                    int node_index = (int16_t)((uint16_t)attachment->change_color_reference - 1);
                    if ( node_index != -1 )
                    {
                        real_rgb_color *node = &object->object.outgoing_change_colors[node_index];
                        colour_r = colour_r * node->n[0];
                        colour_g = node->n[1] * colour_g;
                        colour_b = node->n[2] * colour_b;
                    }
                }

                vertex[4] = texture_offset_u;
                float half_width = width * 0.5f;
                vertex[5] = texture_v_far;
                float *point_position = point->position.n;
                vertex[10] = texture_offset_u;
                vertex[11] = contrail->texture_offset_v;
                unsigned int render_type = (uint16_t)contrail_definition->render_type;
                /* render_type values are $A95FA4AC... _contrail_render_type_* (vertical/horizontal/media/ground/viewer) */
                average_position.n[0] = point->position.n[0] + average_position.n[0];
                average_position.n[1] = point->position.n[1] + average_position.n[1];
                average_position.n[2] = point->position.n[2] + average_position.n[2];

                if ( render_type > _contrail_render_type_viewer )
                {
                    rasterizer_globals.current_lock_operation = _rasterizer_lock_none;
                    return;
                }

                real_vector3d orientation;
                if ( render_type == _contrail_render_type_horizontal || render_type == _contrail_render_type_media )
                {
                    /* offset perpendicular to the screen-space (x,y) tangent to the neighbouring point;
                     * the binary applies perp_a to X and perp_b to Y */
                    float perp_a, perp_b;
                    if ( previous_point )
                    {
                        perp_a = previous_point->position.n[1] - point->position.n[1];
                        perp_b = *point_position - previous_point->position.n[0];
                    }
                    else
                    {
                        contrail_point_datum *next = (contrail_point_datum *)contrail_point_data->data + (uint16_t)point->next_contrail_point_index;
                        perp_a = point->position.n[1] - next->position.n[1];
                        perp_b = next->position.n[0] - *point_position;
                    }
                    float length = sqrtf(perp_b * perp_b + perp_a * perp_a);
                    if ( fabsf(length) >= 0.000099999997f )
                    {
                        float inv = 1.0f / length;
                        perp_b = inv * perp_b;
                        perp_a = inv * perp_a;
                    }
                    float offset = perp_a * half_width;
                    vertex[0] = *point_position - offset;
                    vertex[1] = point->position.n[1] - perp_b * half_width;
                    vertex[2] = point->position.n[2];
                    vertex[6] = *point_position + offset;
                    vertex[7] = point->position.n[1] + perp_b * half_width;
                    vertex[8] = point->position.n[2];
                    if ( apply_orientation )
                    {
                        orientation.n[0] = global_up3d->n[0];
                        orientation.n[1] = global_up3d->n[1];
                        orientation.n[2] = global_up3d->n[2];
                    }
                }
                else if ( render_type == _contrail_render_type_ground )
                {
                    rasterizer_globals.current_lock_operation = _rasterizer_lock_none;
                    return;
                }
                else if ( render_type == _contrail_render_type_viewer )
                {
                    /* camera-facing: cross the segment direction with the eye vector */
                    float ax, ay, az, bx, by, bz;
                    if ( previous_point )
                    {
                        ax = previous_point->position.n[0]; ay = previous_point->position.n[1]; az = previous_point->position.n[2];
                        bx = *point_position;   by = point->position.n[1]; bz = point->position.n[2];
                    }
                    else
                    {
                        ax = *point_position;   ay = point->position.n[1]; az = point->position.n[2];
                        contrail_point_datum *next = (contrail_point_datum *)contrail_point_data->data + (uint16_t)point->next_contrail_point_index;
                        bx = next->position.n[0]; by = next->position.n[1]; bz = next->position.n[2];
                    }
                    float dx = bx - ax, dy = by - ay, dz = bz - az;
                    float ex = render.camera.position.n[0] - ax;
                    float ey = render.camera.position.n[1] - ay;
                    float ez = render.camera.position.n[2] - az;
                    real_vector3d facing_normal;
                    facing_normal.n[0] = dz * ey - dy * ez;
                    facing_normal.n[2] = dy * ex - dx * ey;
                    facing_normal.n[1] = dx * ez - dz * ex;
                    normalize3d(&facing_normal);
                    vertex[0] = *point_position - facing_normal.n[0] * half_width;
                    vertex[1] = point->position.n[1] - facing_normal.n[1] * half_width;
                    vertex[2] = point->position.n[2] - facing_normal.n[2] * half_width;
                    vertex[6] = facing_normal.n[0] * half_width + *point_position;
                    vertex[7] = facing_normal.n[1] * half_width + point->position.n[1];
                    vertex[8] = facing_normal.n[2] * half_width + point->position.n[2];
                    if ( apply_orientation )
                    {
                        orientation.n[0] = facing_normal.n[2] * dy - facing_normal.n[1] * dz;
                        orientation.n[1] = facing_normal.n[0] * dz - facing_normal.n[2] * dx;
                        orientation.n[2] = facing_normal.n[1] * dx - facing_normal.n[0] * dy;
                        normalize3d(&orientation);
                    }
                }
                else
                {
                    /* render_type == 0: vertical ribbon (offset along world Z) */
                    vertex[0] = *point_position;
                    vertex[1] = point->position.n[1];
                    vertex[2] = point->position.n[2] - width * 0.5f;
                    vertex[6] = *point_position;
                    vertex[7] = point->position.n[1];
                    vertex[8] = point->position.n[2] + width * 0.5f;
                    if ( apply_orientation )
                    {
                        if ( previous_point )
                        {
                            orientation.n[0] = previous_point->position.n[1] - point->position.n[1];
                            orientation.n[1] = *point_position - previous_point->position.n[0];
                            orientation.n[2] = 0.0f;
                        }
                        else
                        {
                            contrail_point_datum *next = (contrail_point_datum *)contrail_point_data->data + (uint16_t)point->next_contrail_point_index;
                            orientation.n[2] = 0.0f;
                            orientation.n[0] = point->position.n[1] - next->position.n[1];
                            orientation.n[1] = next->position.n[0] - *point_position;
                        }
                        normalize3d(&orientation);
                    }
                }

                /* fade, pack colour, emit and advance */
                float fade = contrail_fade(contrail_definition, framebuffer_fade_mode, &point->position, &orientation) * colour_a;
                if ( fade >= 0.0f )
                {
                    if ( fade > 1.0f )
                        fade = 1.0f;
                }
                else
                {
                    fade = 0.0f;
                }
                previous_point = point;
                texture_offset_u = texture_u_step + texture_offset_u;
                int packed_color = ((unsigned char)(int)(fade * 255.0f) << 24)
                                 | ((unsigned char)(int)(colour_r * 255.0f) << 16)
                                 | ((unsigned char)(int)(colour_g * 255.0f) << 8)
                                 | (unsigned char)(int)(colour_b * 255.0f);
                /* genuine flat dynamic vertex buffer (6 floats/vertex, 2 verts/record); slots [3] and [9]
                 * are the two vertices' packed-ARGB colour DWORDs written into the float array — no DB struct. */
                int *vertex_dwords = (int *)vertex;
                vertex_dwords[9] = packed_color;
                vertex_dwords[3] = packed_color;
                vertex += 12;
                point_index = point->next_contrail_point_index;
                if ( point_index == -1 )
                    break;
            }
        }

        /* wire up the strip and submit */
        float *first_vertex = &vertex[-6 * vertex_count];
        if ( (contrail_definition->flags & (1u << _contrail_first_point_unfaded_bit)) == 0 )
        {
            /* no head fade-in: force the first cross-section's alpha to zero */
            unsigned char *first_vertex_bytes = (unsigned char *)first_vertex;
            first_vertex_bytes[12] = 0;
            first_vertex_bytes[36] = 0;
        }
        if ( (contrail_definition->flags & (1u << _contrail_last_point_unfaded_bit)) == 0 )
        {
            /* no tail fade-out: force the last cross-section's alpha to zero */
            *((unsigned char *)vertex - 12) = 0;
            *((unsigned char *)vertex - 36) = 0;
        }
        if ( segment_count > 0 )
        {
            rasterizer_triangle *tri = triangle;
            for ( int s = 0; s < segment_count; ++s )
            {
                tri[0].vertex_indices[0] = (int16_t)(2 * s);
                tri[0].vertex_indices[1] = 2 * s + 1;
                tri[0].vertex_indices[2] = 2 * s + 2;
                tri[1].vertex_indices[0] = 2 * s + 2;
                tri[1].vertex_indices[1] = 2 * s + 1;
                tri[1].vertex_indices[2] = 2 * s + 3;
                tri += 2;
            }
        }
        int point_count = contrail->contrail_point_counts[instance_index];
        average_position.n[0] = (1.0f / point_count) * average_position.n[0];
        average_position.n[1] = (1.0f / point_count) * average_position.n[1];
        average_position.n[2] = (1.0f / point_count) * average_position.n[2];
        rasterizer_dynamic_triangles_unlock(triangle_handle);
        rasterizer_dynamic_vertices_unlock(vertex_handle);
        rasterizer_dynamic_unlit_geometry_draw((const shader *)&contrail_definition->shader, bitmap, 0, triangle_handle, vertex_handle, (int16_t)(2 * segment_count), &average_position, 0);
        rasterizer_dynamic_triangles_delete(triangle_handle);
        rasterizer_dynamic_vertices_delete(vertex_handle);
    }

    rasterizer_globals.current_lock_operation = _rasterizer_lock_none;
}
