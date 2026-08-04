/* sample_lightmap @0x836F76A0 — sample a structure surface's baked lightmap at barycentric coords
 * (s,t) and write the result to `out_color`. Reads the three vertices' lightmap texcoords (compressed
 * or uncompressed format selected by the material's vertex buffer type), interpolates them, samples
 * the lightmap bitmap, and unpacks the pixel to RGB.
 *
 * Signature deviation: the decompiler shows two extra parameters (lightmap_sample/r6, a7/r7) before
 * the real out color and names the out `a8`. Those two are the GPR slots reserved by the float args
 * s,t under the PPC ABI (Hex-Rays does not model the FPR/GPR pairing); the real out is r8. */

#include "headers/structure_material.h"
#include "headers/structure_surface.h"
#include "headers/bitmap_data.h"
#include "headers/environment_vertex_uncompressed.h"
#include "headers/environment_lightmap_vertex_compressed.h"
#include "headers/environment_lightmap_vertex_uncompressed.h"
#include "headers/real_point2d.h"
#include "headers/real_rgb_color.h"
#include "headers/rasterizer_vertex_type.h"

extern unsigned int rasterizer_bitmap_2d_get_pixel(const bitmap_data *bitmap, const real_point2d *point, float lod);
extern real_rgb_color *pixel32_to_real_rgb_color(unsigned int pixel, real_rgb_color *color);
extern void environment_lightmap_vertex_compressed_get_texcoord(const environment_lightmap_vertex_compressed *vertex, real_point2d *texcoord);

void sample_lightmap(const structure_material *material, const bitmap_data *bitmap,
        const structure_surface *surface, float s, float t, real_rgb_color *out_color)
{
    int type = material->vertices.type;
    real_point2d tc0, tc1, tc2;
    real_point2d sample_uv;
    unsigned int pixel;

    if ( type == _rasterizer_vertex_type_environment_compressed )
    {
        /* lightmap vertices follow the 32-byte compressed render vertices */
        const environment_lightmap_vertex_compressed *lm =
            (const environment_lightmap_vertex_compressed *)material->compressed_vertex_data.address
            + 4 * material->vertices.count;
        environment_lightmap_vertex_compressed_get_texcoord(&lm[surface->vertex_indices[0]], &tc0);
        environment_lightmap_vertex_compressed_get_texcoord(&lm[surface->vertex_indices[1]], &tc1);
        environment_lightmap_vertex_compressed_get_texcoord(&lm[surface->vertex_indices[2]], &tc2);
    }
    else if ( type == _rasterizer_vertex_type_environment_uncompressed
           || type == _rasterizer_vertex_type_environment_uncompressed_ff )
    {
        /* uncompressed lightmap vertices (20-byte stride, texcoord at +12) follow the 56-byte
         * environment_vertex_uncompressed render vertices. The decompiler renders the ×20 stride
         * as 4*v + 4*ROL4(v,2). */
        const environment_lightmap_vertex_uncompressed *lm =
            (const environment_lightmap_vertex_uncompressed *)
                ((const environment_vertex_uncompressed *)material->uncompressed_vertex_data.address
                + material->vertices.count);
        tc0 = lm[surface->vertex_indices[0]].texcoord;
        tc1 = lm[surface->vertex_indices[1]].texcoord;
        tc2 = lm[surface->vertex_indices[2]].texcoord;
    }

    sample_uv.n[0] = (((tc2.n[0] - tc0.n[0]) * t)
                   + ((tc1.n[0] - tc0.n[0]) * s)) + tc0.n[0];
    sample_uv.n[1] = (((tc1.n[1] - tc0.n[1]) * s)
                   + ((tc2.n[1] - tc0.n[1]) * t)) + tc0.n[1];

    pixel = rasterizer_bitmap_2d_get_pixel(bitmap, &sample_uv, 1.0);
    pixel32_to_real_rgb_color(pixel, out_color);
}
