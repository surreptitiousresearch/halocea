/* interface_draw_bitmap_modulated_p32 @0x836A7348 — draw a single bitmap as a screen-space quad at pixel
 * `point`, rotated by `theta` radians and scaled by `scale` about the bitmap's registration_point, with every
 * vertex modulated by `modulated_color` (raw ARGB, passed straight through) and drawn with framebuffer blend
 * function `shader_type`. When `clip` is NULL the source rect defaults to the bitmap's full pixel extent
 * {0, width, 0, height}. Builds four dynamic_screen_vertex and submits them through the pseudo dynamic screen
 * quad path.
 *
 * DEVIATION: Hex-Rays emitted "local variable allocation has failed" and heavy OVERLAPPED register punning —
 * reconstructed from disasm. FPR-shadow ABI (catalog class 1): f1=scale, f2=theta burn the r6/r7 GPR slots,
 * pushing modulated_color into r8 and shader_type into r9; the DB/PDB 7-arg prototype is authoritative and is
 * used verbatim. Register saves at entry fix the mapping: r30=clip, r27=shader_type, f31=scale, f29=theta,
 * r29=bitmap, r31=point, r28=modulated_color.
 * DEVIATION: unlike the normalized-UV interface_draw_bitmap sibling, the source rect here is in pixel space,
 * so the local offset is (corner - registration) * scale directly and the texcoords are the raw pixel corners.
 * DEVIATION: position.n[2] (z) is deliberately left unwritten — the shipped loop issues only five stores per
 * vertex (pos.x, pos.y, color, u, v); reproduced faithfully. */

#include <stdint.h>
#include "headers/bitmap_data.h"
#include "headers/point2d.h"
#include "headers/real_rectangle2d.h"
#include "headers/dynamic_screen_vertex.h"
#include "headers/rasterizer_dynamic_screen_geometry_parameters.h"
#include <math.h>
#include <string.h>

extern void rasterizer_psuedo_dynamic_screen_quad_draw(rasterizer_dynamic_screen_geometry_parameters *parameters, dynamic_screen_vertex *vertices);

void interface_draw_bitmap_modulated_p32(const bitmap_data *bitmap, const point2d *point, const real_rectangle2d *clip, float scale, float theta, unsigned int modulated_color, int16_t shader_type)
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
        verts[i].color         = modulated_color;
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
