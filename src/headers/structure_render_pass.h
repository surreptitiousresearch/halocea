#pragma once
#include <stdint.h>
/* structure_render_pass @0x837C65B0 — shared prototype for the BSP environment render dispatcher and the
 * callback signatures it invokes. The structure_render_* entry points each call this with a different
 * draw_triangles / draw_transparent_triangles callback for their pass. Forward declarations only; the
 * callbacks treat the geometry types opaquely here.
 * DEVIATION: draw_transparent_triangles's REAL impl (rasterizer_environment_transparent_geometry_submit)
 * has an 11-arg signature, but structure_render_pass's single call site casts the pointer down to 8 args
 * and only ever supplies those — the trailing plane/offset/render_lighting/geometry_flags are never set
 * up by this caller. Declared here with the 8 args actually passed. */

struct shader;
struct bitmap_data;
struct vertex_buffer;
union real_point3d;   /* real_point3d is a union tag, not a struct */

extern void structure_render_pass(
    int *surface_indices,
    int16_t surface_count,
    int dynamic_triangles_index,
    void (*begin_lightmap)(struct bitmap_data *),
    void (*draw_triangles)(const struct shader *, int16_t, int, int, int, const struct vertex_buffer *),
    void (*end_lightmap)(void),
    void (*draw_transparent_triangles)(const struct shader *, int16_t, struct bitmap_data *, int, int,
                                       int, const struct vertex_buffer *, const union real_point3d *));
