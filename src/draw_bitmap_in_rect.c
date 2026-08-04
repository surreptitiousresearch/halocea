/* draw_bitmap_in_rect @0x83730A58 — blit a (sub-rectangle of a) bitmap into a destination screen rectangle as a
 * single textured quad, optionally clipped to a clip rectangle. The source sub-rectangle (bitmap_rect, in texel
 * units; defaults to the whole bitmap) determines the texture-coordinate extents; the destination rectangle gives
 * the screen corners. When a clip rectangle is supplied the four destination edges are clamped to it (positions
 * only — the texture coordinates are intentionally NOT re-derived, matching the original). The quad is emitted
 * through the rasterizer's pseudo dynamic-screen-geometry path with a freshly built single-map parameter block.
 *
 * Vertex winding is TL, TR, BR, BL; every vertex carries the same packed ARGB color.
 *
 * DEVIATIONS (decompiler reported "local variable allocation has failed"; reconstructed from disassembly):
 *   - The decompiler packed args r7 (argb) and r8 (multitexture_params) into one __int64 and rendered the vertex
 *     color as HIDWORD(multitexture_params). The store is `stwx r7,...`: the color is plainly `argb`.
 *   - `multitexture_params` and `no_plasma` are dead in this build — the body builds its own parameter block and
 *     never reads either argument. Kept in the signature (DB prototype) and marked unused.
 *   - The decompiler's `*(_QWORD *)v31 = 0x1100000000LL` fill loop is a zeroing memset of the parameter block
 *     (ctr = 17 qwords + 1 trailing dword == sizeof params); restored as an explicit zero-init.
 *   - The three 0.9 stores below the vertex array are dead (leftover tint for the disabled plasma path); kept
 *     faithfully with a note.
 *   - Every int->float corner/extent conversion is an fcfid artifact (std/lfd/fcfid/frsp); restored as casts. */

#include <stdint.h>
#include "headers/bitmap_data.h"
#include "headers/rectangle2d.h"
#include "headers/dynamic_screen_vertex.h"
#include "headers/rasterizer_dynamic_screen_geometry_parameters.h"

extern void rasterizer_psuedo_dynamic_screen_quad_draw(rasterizer_dynamic_screen_geometry_parameters *parameters, dynamic_screen_vertex *vertices);

void draw_bitmap_in_rect(bitmap_data *bitmap, rectangle2d *rect, rectangle2d *bitmap_rect, rectangle2d *clip_rect,
                         unsigned int argb, rasterizer_dynamic_screen_geometry_parameters *multitexture_params,
                         uint8_t no_plasma)
{
    (void)multitexture_params;  /* unused in this build */
    (void)no_plasma;            /* unused in this build */

    if (!bitmap || !rect)
        return;

    /* dead tint for the disabled plasma path — stored by the original, never read */
    float unused_plasma_tint[3];
    unused_plasma_tint[0] = 0.9f;
    unused_plasma_tint[1] = 0.9f;
    unused_plasma_tint[2] = 0.9f;

    /* default the source sub-rectangle to the whole bitmap */
    rectangle2d full_bitmap_rect;
    if (!bitmap_rect)
    {
        full_bitmap_rect.y0 = 0;
        full_bitmap_rect.x0 = 0;
        full_bitmap_rect.y1 = bitmap->height;
        full_bitmap_rect.x1 = bitmap->width;
        bitmap_rect = &full_bitmap_rect;
    }

    /* destination corners, as floats */
    float left   = (float)rect->x0;
    float right  = (float)rect->x1;
    float top    = (float)rect->y0;
    float bottom = (float)rect->y1;

    /* clamp the destination edges to the clip rectangle (positions only) */
    if (clip_rect)
    {
        if (clip_rect->x0 > rect->x0)
            left = (float)clip_rect->x0;
        if (clip_rect->x1 < rect->x1)
            right = (float)clip_rect->x1;
        if (clip_rect->y0 > rect->y0)
            top = (float)clip_rect->y0;
        if (clip_rect->y1 < rect->y1)
            bottom = (float)clip_rect->y1;
    }

    /* texture-coordinate extents: source sub-rect size as a fraction of the bitmap, capped at 1.0 */
    float bitmap_width  = (float)bitmap->width;
    if (bitmap_width < 1.0f)
        bitmap_width = 1.0f;
    float u_max = (float)(bitmap_rect->x1 - bitmap_rect->x0) / bitmap_width;
    if (u_max > 1.0f)
        u_max = 1.0f;

    float bitmap_height = (float)bitmap->height;
    if (bitmap_height < 1.0f)
        bitmap_height = 1.0f;
    float v_max = (float)(bitmap_rect->y1 - bitmap_rect->y0) / bitmap_height;
    if (v_max > 1.0f)
        v_max = 1.0f;

    /* the four destination corners, interleaved (x,y) as the original keeps them: TL, TR, BR, BL */
    real_point2d corner[4];
    corner[0].n[0] = left;   corner[0].n[1] = top;
    corner[1].n[0] = right;  corner[1].n[1] = top;
    corner[2].n[0] = right;  corner[2].n[1] = bottom;
    corner[3].n[0] = left;   corner[3].n[1] = bottom;

    dynamic_screen_vertex vertices[4];
    for (int i = 0; i < 4; i = (__int16)(i + 1))
    {
        vertices[i].color = argb;
        vertices[i].texcoord.n[0] = (i % 3 == 0) ? 0.0f : u_max;  /* u: 0 on the left corners (0,3) */
        vertices[i].texcoord.n[1] = (i <= 1) ? 0.0f : v_max;      /* v: 0 on the top corners (0,1) */
        vertices[i].position.n[0] = corner[i].n[0];
        vertices[i].position.n[1] = corner[i].n[1];
        vertices[i].position.n[2] = 0.0f;
    }

    /* single-map parameter block: just this bitmap, identity scales, no blend, bilinear */
    rasterizer_dynamic_screen_geometry_parameters parameters;
    for (unsigned char *p = (unsigned char *)&parameters;
         p < (unsigned char *)&parameters + sizeof(parameters); ++p)
        *p = 0;  /* zero-init the whole block (original: 17 qword stores + 1 dword) */

    parameters.meter_parameters = nullptr;
    parameters.map[0] = bitmap;
    parameters.map_scale[0].n[0] = 1.0f;
    parameters.map_scale[0].n[1] = 1.0f;
    parameters.map_texture_scale[0].n[0] = 1.0f;
    parameters.map_texture_scale[0].n[1] = 1.0f;
    parameters.framebuffer_blend_function = 0;
    parameters.point_sampled = 0;

    rasterizer_psuedo_dynamic_screen_quad_draw(&parameters, vertices);
}
