/* structure_render_dynamic_triangles_from_bitvector @0x837C6300 — expand a per-surface bit vector into a
 * dynamic triangle list. For each set bit (one BSP surface), append the surface index to the output list and
 * copy that surface's three vertex indices into the triangle buffer.
 *
 * The database prototype for this function is mislabeled (it lists a leading surface_count and a trailing
 * triangles argument); register evidence and the call site (structure_render_build_dynamic_triangles) show
 * the real argument order is (output surface index list, surface bit vector, triangle output buffer). */

#include <stdint.h>
#include "headers/structure_bsp.h"
#include "headers/structure_surface.h"
#include "headers/rasterizer_triangle.h"

void structure_render_dynamic_triangles_from_bitvector(int *out_surface_indices,
    const unsigned int *surface_bit_vector, rasterizer_triangle *triangles)
{
    int16_t triangle_count = 0;
    structure_bsp *bsp = global_structure_bsp;

    if (!triangles || bsp->surfaces.count <= 0)
        return;

    int *out_index = out_surface_indices - 1;   /* pre-increment store */
    int surface_index = 0;
    do
    {
        unsigned int word = *surface_bit_vector;
        if (word)
        {
            for (int bit = 0; bit < 32; ++bit)
            {
                if (surface_index >= bsp->surfaces.count)
                    break;
                if (((1u << bit) & word) != 0)
                {
                    const structure_surface *surface =
                        (const structure_surface *)bsp->surfaces.address + surface_index;
                    *++out_index = surface_index;
                    rasterizer_triangle *triangle = &triangles[triangle_count++];
                    triangle->vertex_indices[0] = surface->vertex_indices[0];
                    triangle->vertex_indices[1] = surface->vertex_indices[1];
                    triangle->vertex_indices[2] = surface->vertex_indices[2];
                }
                ++surface_index;
            }
        }
        else
        {
            surface_index += 32;
        }
        ++surface_bit_vector;
    } while (surface_index < bsp->surfaces.count);
}
