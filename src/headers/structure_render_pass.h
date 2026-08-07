#pragma once
#include <stdint.h>
/* structure_render_pass @0x837C65B0 — shared prototype for the BSP environment render dispatcher and the
 * callback signatures it invokes. The structure_render_* entry points each call this with a different
 * draw_triangles / draw_transparent_triangles callback for their pass. Forward declarations only; the
 * callbacks treat the geometry types opaquely here.
 * CAVEAT (2026-08-07): the 8-arg draw_transparent_triangles below is INCOMPLETE, and the note that used
 * to stand here — "the trailing plane/offset/render_lighting/geometry_flags are never set up by this
 * caller" — is refuted by the disassembly. structure_render_pass sets all four up immediately before its
 * `bctrl` at 0x837C67D0: four stack stores at r1+0x54/0x5C/0x64/0x6C (0x837C679C-0x837C67BC) on top of
 * the eight register args, i.e. a 12-argument call. Resolved so far, against types_members
 * structure_material: arg9 `const real_plane3d *` = (flags & 1) ? &material->plane (+0x9C) : r20;
 * arg11 `const render_lighting *` = &material->lighting (+0x28); arg10 `const real_vector3d *` =
 * (flags & 2) ? r15+0xC : *(<stack var_A0> + 0x6F54); arg12 `unsigned int` = r20. r15, r20 and the
 * var_A0 base still need deriving before the signature can be widened, so the declaration stays at the
 * 8 args the reconstruction actually passes rather than promising four values it cannot yet supply.
 * src/structure_render_shadow.c already declares the 12-arg form; the two disagree on purpose until
 * this is closed. */

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
