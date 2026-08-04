/* structure_render_dynamic_triangles_from_bitvector @0x837C6300 — expand a per-surface bit vector into a
 * dynamic triangle list. For each set bit (one BSP surface), append the surface index to the output list and
 * copy that surface's three vertex indices into the triangle buffer.
 *
 * The database prototype for this function is mislabeled (it lists a leading surface_count and a trailing
 * triangles argument); register evidence and the call site (structure_render_build_dynamic_triangles) show
 * the real argument order is (output surface index list, surface bit vector, triangle output buffer). */

#include "headers/structure_bsp.h"
#include "headers/rasterizer_triangle.h"

void structure_render_dynamic_triangles_from_bitvector(int *out_surface_indices,
    const unsigned int *surface_bit_vector, rasterizer_triangle *triangles)
{
    __int16 triangle_count = 0;
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
            int surface_byte_offset = 6 * surface_index;
            for (int bit = 0; bit < 32; ++bit)
            {
                if (surface_index >= bsp->surfaces.count)
                    break;
                if (((1u << bit) & word) != 0)
                {
                    unsigned __int16 *surface =
                        (unsigned __int16 *)((char *)bsp->surfaces.address + surface_byte_offset);
                    *++out_index = surface_index;
                    rasterizer_triangle *triangle = &triangles[triangle_count++];
                    triangle->vertex_indices[0] = surface[0];
                    triangle->vertex_indices[1] = surface[1];
                    triangle->vertex_indices[2] = surface[2];
                }
                ++surface_index;
                surface_byte_offset += 6;
            }
        }
        else
        {
            surface_index += 32;
        }
        ++surface_bit_vector;
    } while (surface_index < bsp->surfaces.count);
}
