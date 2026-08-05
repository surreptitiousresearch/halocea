/* structure_render_build_dynamic_triangles @0x837C6490 — allocate and fill a dynamic triangle buffer for a
 * set of BSP environment surfaces, returning the buffer index (-1 on failure / empty). When surface_flags is
 * supplied the triangle list is built from the surface bit-vector; otherwise the surface_indices are sorted
 * and each surface's three vertex indices are copied straight into the locked buffer. */

#include <stdint.h>
#include "headers/structure_bsp.h"
#include "headers/structure_surface.h"
#include "headers/rasterizer_triangle.h"
#include "headers/blam_data_globals.h"

extern int rasterizer_dynamic_triangles_new(int count);
extern rasterizer_triangle *rasterizer_dynamic_triangles_lock(int dynamic_triangle_buffer_index);
extern void rasterizer_dynamic_triangles_unlock(int dynamic_triangle_buffer_index);
extern void qsort_4byte(void *base, unsigned int num, int (*compare)(int, int));
extern int compare_surface_indices(int index1, int index2);
/* DB prototype for this callee is mislabeled (lists a surface_count + a 4th triangles arg); the call site
 * passes exactly (surface_indices, surface_flags, locked_buffer) in r3/r4/r5. */
extern void structure_render_dynamic_triangles_from_bitvector(int *out_surface_indices, const unsigned int *surface_bit_vector, rasterizer_triangle *triangles);


int structure_render_build_dynamic_triangles(int *surface_indices, int16_t surface_count, unsigned int *surface_flags)
{
    if (surface_count <= 0)
        return -1;

    int triangle_buffer_index = rasterizer_dynamic_triangles_new(surface_count);
    if (triangle_buffer_index == -1)
    {
        once = once != 0 ? 0 : once;
        return -1;
    }

    rasterizer_triangle *triangles = rasterizer_dynamic_triangles_lock(triangle_buffer_index);

    if (surface_flags)
    {
        structure_render_dynamic_triangles_from_bitvector(surface_indices, surface_flags, triangles);
        rasterizer_dynamic_triangles_unlock(triangle_buffer_index);
        return triangle_buffer_index;
    }

    structure_bsp *bsp = global_structure_bsp;
    if (triangles)
    {
        qsort_4byte(surface_indices, surface_count, compare_surface_indices);
        for (int i = 0; i < surface_count; i = (int16_t)(i + 1))
        {
            uint16_t *surface = ((structure_surface *)bsp->surfaces.address)[surface_indices[i]].vertex_indices;
            triangles[i].vertex_indices[0] = surface[0];
            triangles[i].vertex_indices[1] = surface[1];
            triangles[i].vertex_indices[2] = surface[2];
        }
    }

    rasterizer_dynamic_triangles_unlock(triangle_buffer_index);
    return triangle_buffer_index;
}
