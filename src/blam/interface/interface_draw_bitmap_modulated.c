/* interface_draw_bitmap_modulated @0x836A70D8 — draw a single bitmap as a screen-space quad at pixel `point`,
 * rotated by `theta` radians and scaled by `scale` about the bitmap's registration_point, with every vertex
 * modulated by `modulated_color`. Identical to interface_draw_bitmap_modulated_p32 (the adjacent sibling)
 * except the modulation color arrives as a real_argb_color (four floats in [0,1]) which is packed to 0xAARRGGBB
 * here (a=n[0], r=n[1], g=n[2], b=n[3], each * 255 and truncated) rather than passed through as a raw ARGB int.
 * When `clip` is NULL the source rect defaults to the bitmap's full pixel extent {0, width, 0, height}. Builds
 * four dynamic_screen_vertex and submits them through the pseudo dynamic screen quad path.
 *
 * DEVIATION: Hex-Rays emitted "local variable allocation has failed" and heavy OVERLAPPED / __int128 register
 * punning — reconstructed from disasm and from the verified p32 sibling. FPR-shadow ABI (catalog class 1):
 * f1=scale, f2=theta burn the r6/r7 GPR slots, pushing modulated_color into r8 and shader_type into r9; the
 * DB/PDB 7-arg prototype is authoritative. Source rect is pixel-space (offset = (corner - registration)*scale,
 * texcoords are the raw pixel corners) and position.n[2] (z) is deliberately left unwritten (five stores per
 * vertex). The params zero-init the decompiler rendered as a `HIDWORD(v29) | 0x1100000000` fill loop is a plain
 * memset-to-0: disasm shows the fill value is r7 (== 0) and 0x11 is merely the loop count (mtctr). */

#include <stdint.h>
#include "headers/bitmap_data.h"
#include "headers/point2d.h"
#include "headers/real_rectangle2d.h"
#include "headers/real_argb_color.h"
#include "headers/dynamic_screen_vertex.h"
#include "headers/rasterizer_dynamic_screen_geometry_parameters.h"
#include <math.h>
#include <string.h>

extern void rasterizer_psuedo_dynamic_screen_quad_draw(const rasterizer_dynamic_screen_geometry_parameters *parameters, dynamic_screen_vertex *vertices);

void interface_draw_bitmap_modulated(const bitmap_data *bitmap, const point2d *point, const real_rectangle2d *clip, float scale, float theta, const real_argb_color *modulated_color, int16_t shader_type)
{
    float width  = (float)bitmap->width;
    float height = (float)bitmap->height;

    real_rectangle2d default_clip = { 0.0f, width, 0.0f, height };
    if ( !clip )
        clip = &default_clip;

    float sin_t = (float)sin(theta);
    float cos_t = (float)cos(theta);

    float reg_x = (float)bitmap->registration_point.x;
    float reg_y = (float)bitmap->registration_point.y;
    float pos_x = (float)point->x;
    float pos_y = (float)point->y;

    unsigned int packed_color =
          ((unsigned int)(int)(modulated_color->n[0] * 255.0f) << 24)
        | ((unsigned int)(int)(modulated_color->n[1] * 255.0f) << 16)
        | ((unsigned int)(int)(modulated_color->n[2] * 255.0f) << 8)
        |  (unsigned int)(int)(modulated_color->n[3] * 255.0f);

    dynamic_screen_vertex verts[4];
    for ( int i = 0; i < 4; ++i )
    {
        float u = ((i + 1) & 2) ? clip->n[1] : clip->n[0];
        float v = (i > 1)       ? clip->n[3] : clip->n[2];

        float local_x_s = (u - reg_x) * scale;
        float local_y_s = (v - reg_y) * scale;

        verts[i].position.n[0] = local_x_s * cos_t - local_y_s * sin_t + pos_x;
        verts[i].position.n[1] = local_x_s * sin_t + local_y_s * cos_t + pos_y;
        /* position.n[2] left unwritten — see header DEVIATION */
        verts[i].color         = packed_color;
        verts[i].texcoord.n[0] = u;
        verts[i].texcoord.n[1] = v;
    }

    rasterizer_dynamic_screen_geometry_parameters params;
    memset(&params, 0, sizeof(params));
    params.map[0]                     = (bitmap_data *)bitmap;
    params.map_scale[0].n[0]          = 1.0f;
    params.map_scale[0].n[1]          = 1.0f;
    params.map_texture_scale[0].n[0]  = 1.0f;
    params.map_texture_scale[0].n[1]  = 1.0f;
    params.framebuffer_blend_function = shader_type;

    rasterizer_psuedo_dynamic_screen_quad_draw(&params, verts);
}
