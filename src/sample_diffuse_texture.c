/* sample_diffuse_texture @0x836F7840 — sample a structure surface's diffuse (base) texture at
 * barycentric coords (s,t) and write the result to `out_color`. Same shape as sample_lightmap but
 * reads the render vertices' diffuse texcoords and samples with a 0.3 LOD bias.
 *
 * Signature deviation: as with sample_lightmap, the decompiler's diffuse_sample/r6 and a7/r7 are the
 * GPR slots reserved by float args s,t; the real out color is r8 (decompiler's a8). */

#include "headers/structure_material.h"
#include "headers/structure_surface.h"
#include "headers/bitmap_data.h"
#include "headers/environment_vertex_compressed.h"
#include "headers/real_point2d.h"
#include "headers/real_rgb_color.h"
#include "headers/rasterizer_vertex_type.h"

extern unsigned int rasterizer_bitmap_2d_get_pixel(const bitmap_data *bitmap, const real_point2d *point, float lod);
extern real_rgb_color *pixel32_to_real_rgb_color(unsigned int pixel, real_rgb_color *color);
extern void environment_vertex_compressed_get_texcoord(const environment_vertex_compressed *vertex, real_point2d *texcoord);

void sample_diffuse_texture(const structure_material *material, const bitmap_data *bitmap,
        const structure_surface *surface, float s, float t, real_rgb_color *out_color)
{
    int type = material->vertices.type;
    real_point2d tc0, tc1, tc2;
    real_point2d sample_uv;
    unsigned int pixel;

    if ( type == _rasterizer_vertex_type_environment_compressed )
    {
        const environment_vertex_compressed *verts =
            (const environment_vertex_compressed *)material->compressed_vertex_data.address;
        environment_vertex_compressed_get_texcoord(&verts[surface->vertex_indices[0]], &tc0);
        environment_vertex_compressed_get_texcoord(&verts[surface->vertex_indices[1]], &tc1);
        environment_vertex_compressed_get_texcoord(&verts[surface->vertex_indices[2]], &tc2);
    }
    else if ( type == _rasterizer_vertex_type_environment_uncompressed
           || type == _rasterizer_vertex_type_environment_uncompressed_ff )
    {
        /* uncompressed render vertices: 7 real_point2d-sized fields (56-byte stride), diffuse
         * texcoord at element +6. */
        const real_point2d *verts = (const real_point2d *)material->uncompressed_vertex_data.address;
        tc0 = verts[7 * surface->vertex_indices[0] + 6];
        tc1 = verts[7 * surface->vertex_indices[1] + 6];
        tc2 = verts[7 * surface->vertex_indices[2] + 6];
    }

    sample_uv.n[0] = (((tc2.n[0] - tc0.n[0]) * t)
                   + ((tc1.n[0] - tc0.n[0]) * s)) + tc0.n[0];
    sample_uv.n[1] = (((tc1.n[1] - tc0.n[1]) * s)
                   + ((tc2.n[1] - tc0.n[1]) * t)) + tc0.n[1];

    pixel = rasterizer_bitmap_2d_get_pixel(bitmap, &sample_uv, 0.30000001f);
    pixel32_to_real_rgb_color(pixel, out_color);
}
