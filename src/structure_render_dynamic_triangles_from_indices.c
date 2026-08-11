/* structure_render_dynamic_triangles_from_indices @0x837C640C — sort surface_indices (via compare_surface_indices)
 * and expand each into a rasterizer_triangle's three vertex indices, read from the current structure BSP's
 * surfaces tag_block (6-byte structure_surface records, same layout as rasterizer_triangle). No-op if
 * `triangles` is null. */

#include <stdint.h>
#include "headers/structure_bsp.h"
#include "headers/rasterizer_triangle.h"

extern void qsort_4byte(int *base, unsigned int num, int (*compare)(int, int));
extern int compare_surface_indices(int index1, int index2);

void structure_render_dynamic_triangles_from_indices(int16_t surface_count, int *surface_indices, rasterizer_triangle *triangles)
{
    if ( !triangles )
        return;

    qsort_4byte(surface_indices, surface_count, compare_surface_indices);

    for ( int16_t i = 0; i < surface_count; i++ )
    {
        /* surfaces are 6-byte structure_surface records, same layout as rasterizer_triangle */
        rasterizer_triangle *surface =
            &((rasterizer_triangle *)global_structure_bsp->surfaces.address)[surface_indices[i]];
        triangles[i] = *surface;
    }
}
