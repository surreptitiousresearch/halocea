/* interface_draw_bitmap @0x836A6EA8 */
/* interface_draw_bitmap 0x836A6EA8 — draw a single bitmap quad at pixel `point`, rotated by
 * `theta` radians around the bitmap's registration_point, scaled by `scale`, with alpha `fade`.
 * When `clip` is NULL the full UV rect {0,1,0,1} is used.  Blend function 7 = alpha_multiply_add.
 * DEVIATION: decompiler emitted "local variable allocation has failed" — reconstructed from
 *   disasm. FPR-shadow ABI: f1(r6)=scale, f2(r7)=theta, f3(r8)=fade; r3=bitmap, r4=point,
 *   r5=clip verified from entry save-sequence (fmr f31,f1 / fmr f26,f2 / mr r29,r3 etc.).
 *   Vertex layout confirmed: offset 0=pos.x, 4=pos.y, 8=z, 0x0C=color, 0x10=u, 0x14=v. */

#include "headers/bitmap_data.h"
#include "headers/point2d.h"
#include "headers/real_rectangle2d.h"
#include "headers/dynamic_screen_vertex.h"
#include "headers/rasterizer_dynamic_screen_geometry_parameters.h"
#include "headers/shader_framebuffer_blend_function.h"
#include <math.h>
#include <string.h>

extern void rasterizer_psuedo_dynamic_screen_quad_draw(const rasterizer_dynamic_screen_geometry_parameters *parameters, dynamic_screen_vertex *vertices);

void interface_draw_bitmap(const bitmap_data *bitmap, const point2d *point,
                           const real_rectangle2d *clip,
                           float scale, float theta, float fade)
{
    real_rectangle2d default_clip = { 0.0f, 1.0f, 0.0f, 1.0f };
    if ( !clip )
        clip = &default_clip;

    float sin_t = (float)sin(theta);
    float cos_t = (float)cos(theta);

    float width  = (float)bitmap->width;
    float height = (float)bitmap->height;
    float reg_x  = (float)bitmap->registration_point.x;
    float reg_y  = (float)bitmap->registration_point.y;
    float pos_x  = (float)point->x;
    float pos_y  = (float)point->y;

    /* alpha byte: fade in [0,1] → [0,255], packed into MSB of white ARGB */
    unsigned int color = ((unsigned int)(int)(fade * 255.0f) << 24) | 0x00FFFFFFu;

    dynamic_screen_vertex verts[4];
    for ( int i = 0; i < 4; ++i )
    {
        float u = ((i + 1) & 2) ? clip->n[1] : clip->n[0];
        float v = (i > 1)       ? clip->n[3] : clip->n[2];

        float local_x_s = (width  * u - reg_x) * scale;
        float local_y_s = (height * v - reg_y) * scale;

        verts[i].position.n[0] = local_x_s * cos_t - local_y_s * sin_t + pos_x;
        verts[i].position.n[1] = local_x_s * sin_t + local_y_s * cos_t + pos_y;
        verts[i].position.n[2] = 0.0f;
        verts[i].color         = color;
        verts[i].texcoord.n[0] = u;
        verts[i].texcoord.n[1] = v;
    }

    rasterizer_dynamic_screen_geometry_parameters params;
    memset(&params, 0, sizeof(params));
    params.map[0]                    = (bitmap_data *)bitmap;
    params.map_scale[0].n[0]         = 1.0f;
    params.map_scale[0].n[1]         = 1.0f;
    params.map_texture_scale[0].n[0] = 1.0f;
    params.map_texture_scale[0].n[1] = 1.0f;
    params.framebuffer_blend_function = _shader_framebuffer_blend_function_alpha_multiply_add;

    rasterizer_psuedo_dynamic_screen_quad_draw(&params, verts);
}
