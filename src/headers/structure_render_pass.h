#pragma once
#include <stdint.h>
/* structure_render_pass @0x837C65B0 — shared prototype for the BSP environment render dispatcher and the
 * callback signatures it invokes. The structure_render_* entry points each call this with a different
 * draw_triangles / draw_transparent_triangles callback for their pass. Forward declarations only; the
 * callbacks treat the geometry types opaquely here.
 * draw_transparent_triangles takes TWELVE parameters: the eight register args plus four Xbox 360 8-byte
 * parameter slots stored at r1+0x54/0x5C/0x64/0x6C immediately before the `bctrl` at 0x837C67D0. The DB
 * prototype for structure_render_pass declares the same twelve, and src/structure_render_shadow.c already
 * spelled it that way; only this header carried the truncated 8-arg form. */

struct shader;
struct bitmap_data;
struct vertex_buffer;
union real_point3d;   /* real_point3d is a union tag, not a struct */
union real_vector3d;  /* likewise real_vector3d */
struct real_plane3d;
struct render_lighting;

extern void structure_render_pass(
    int *surface_indices,
    int16_t surface_count,
    int dynamic_triangles_index,
    void (*begin_lightmap)(struct bitmap_data *),
    void (*draw_triangles)(const struct shader *, int16_t, int, int, int, const struct vertex_buffer *),
    void (*end_lightmap)(void),
    void (*draw_transparent_triangles)(const struct shader *, int16_t, const struct bitmap_data *, int, int,
                                       int, const struct vertex_buffer *, const union real_point3d *,
                                       const struct real_plane3d *, const union real_vector3d *,
                                       const struct render_lighting *, unsigned int));
