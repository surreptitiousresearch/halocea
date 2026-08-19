/* hud_draw_bitmap_internal @0x8379EA20 */
#include <math.h>
#include <string.h>
#include "headers/bitmap_data.h"
#include "headers/point2d.h"
#include "headers/real_rectangle2d.h"
#include "headers/real_vector2d.h"
#include "headers/dynamic_screen_vertex.h"
#include "headers/rasterizer_dynamic_screen_geometry_parameters.h"
#include "headers/shader_framebuffer_blend_function.h"

extern void rasterizer_psuedo_dynamic_screen_quad_draw(const rasterizer_dynamic_screen_geometry_parameters *parameters, dynamic_screen_vertex *vertices);

/* DEVIATION: DB proto uses float theta; decompiler shows double via FPR-shadow widening.
 * DEVIATION: the float arg theta (arg 7, f1) shadows GPR r9 (skipped), so the color arg lands
 * in r10 — there is NO separate r9 color argument. A prior reconstruction split the single r10
 * color into a phantom r9 `color` + r10 `vertex_color`; removed. Disasm: prologue reads only
 * r3-r8,r10,f1 (r9 is scratch), and stwx r31(<-r10) writes the vertex color @0x8379EB2C. */
void hud_draw_bitmap_internal(
        rasterizer_meter_parameters *meter_parameters,
        const bitmap_data           *bitmap,
        const point2d               *point,
        const real_rectangle2d      *clip,
        const real_rectangle2d      *bounds,
        const real_vector2d         *xy_scale,
        float                        theta,
        unsigned int                 color)
{
    float sin_theta = (float)sin((double)theta);
    float cos_theta = (float)cos((double)theta);
    float scale_x   = xy_scale->n[0];
    float scale_y   = xy_scale->n[1];
    float px        = (float)point->x;
    float py        = (float)point->y;

    /* decompiler says v37[3] due to local-alloc-failed; quad requires 4 vertices */
    dynamic_screen_vertex quad_vertices[4];
    int i = 0;
    do
    {
        int next_i      = i + 1;
        int use_n1      = next_i & 2;  /* (v24+1)&2: selects n[1] vs n[0] for x */
        float clip_x    = use_n1 ? clip->n[1]   : clip->n[0];
        float clip_y    = (i <= 1) ? clip->n[2]  : clip->n[3];
        float bounds_x  = use_n1 ? bounds->n[1] : bounds->n[0];
        float bounds_y  = (i <= 1) ? bounds->n[2] : bounds->n[3];

        quad_vertices[i].position.n[2] = 0.0f;
        quad_vertices[i].texcoord.n[0] = clip_x;
        quad_vertices[i].texcoord.n[1] = clip_y;
        quad_vertices[i].color         = color;
        quad_vertices[i].position.n[0] = (bounds_x * cos_theta - bounds_y * sin_theta) * scale_x + px;
        quad_vertices[i].position.n[1] = (bounds_y * cos_theta + bounds_x * sin_theta) * scale_y + py;
        i = next_i;
    }
    while (i < 4);

    rasterizer_dynamic_screen_geometry_parameters geometry_params;
    memset(&geometry_params, 0, sizeof(geometry_params));
    geometry_params.meter_parameters           = meter_parameters;
    geometry_params.map[0]                     = (bitmap_data *)bitmap;
    geometry_params.framebuffer_blend_function = _shader_framebuffer_blend_function_alpha_multiply_add;
    geometry_params.point_sampled              = 0;
    geometry_params.map_texture_scale[0].n[1]  = 1.0f;
    geometry_params.map_texture_scale[0].n[0]  = 1.0f;
    geometry_params.map_scale[0].n[1]          = 1.0f;
    geometry_params.map_scale[0].n[0]          = 1.0f;
    rasterizer_psuedo_dynamic_screen_quad_draw(&geometry_params, quad_vertices);
}
