/* structure_render_shadow 0x837C7010 — render the structure (BSP) geometry that falls inside an object's
 * shadow volume. Gathers the BSP surfaces overlapping the shadow bounds (a sphere + an oriented box given as a
 * set of clip planes) into a local index buffer, builds a dynamic triangle batch from them, then runs a render
 * pass that draws those triangles through rasterizer_environment_shadow_draw, and frees the batch.
 *
 * DEVIATION: the decompiler grew a phantom 6th parameter (a6) and mistyped bounding_polyhedra as
 * real_rectangle3d* — both artifacts of the float-argument GPR-shadow ABI. From the disassembly the
 * structure_visibility_build_surfaces call is (buffer, 4096, center, radius, bounding_box, polyhedra_count,
 * polyhedra, cluster_count = 0, cluster_indices = NULL); the "a6" was the literal 0 cluster_count. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_rectangle3d.h"
#include "headers/real_plane3d.h"
#include "headers/real_vector3d.h"
#include "headers/shader.h"
#include "headers/vertex_buffer.h"
#include "headers/bitmap_data.h"
#include "headers/render_lighting.h"

#include "headers/shader.h"
#include "headers/vertex_buffer.h"
extern int16_t structure_visibility_build_surfaces(int *surface_indices, int16_t maximum_count, const real_point3d *bounding_sphere_center, float bounding_sphere_radius, const real_rectangle3d *bounding_box, int16_t bounding_surface_count, const real_plane3d *bounding_surfaces, int16_t cluster_count, int16_t *cluster_indices);
extern int structure_render_build_dynamic_triangles(int *surface_indices, int16_t surface_count, unsigned int *surface_flags);
extern void rasterizer_environment_shadow_draw(const shader *shader, int16_t shader_permutation_index, int dynamic_triangle_buffer_index, int first_triangle_index, int triangle_count, const vertex_buffer *vertex_buffer);
extern void structure_render_pass(int *surface_indices, __int16 surface_count, int dynamic_triangles_index,
    void (*begin_lightmap)(bitmap_data *),
    void (*draw_triangles)(const shader *, __int16, int, int, int, const vertex_buffer *),
    void (*end_lightmap)(void),
    void (*draw_transparent_triangles)(const shader *, __int16, const bitmap_data *, int, int, int,
        const vertex_buffer *, const real_point3d *, const real_plane3d *, const real_vector3d *,
        const render_lighting *, unsigned int));
extern void rasterizer_dynamic_triangles_delete(int dynamic_triangle_buffer_index);

void structure_render_shadow(const real_point3d *bounding_sphere_center, float bounding_radius, const real_rectangle3d *bounding_box, int16_t bounding_polyhedra_count, const real_plane3d *bounding_polyhedra)
{
    int surface_indices[4096];
    __int16 surface_count = structure_visibility_build_surfaces(
        surface_indices, 4096, bounding_sphere_center, bounding_radius, bounding_box,
        bounding_polyhedra_count, bounding_polyhedra, 0, nullptr);

    int dynamic_triangles_index = structure_render_build_dynamic_triangles(surface_indices, surface_count, nullptr);
    if ( dynamic_triangles_index != -1 )
    {
        structure_render_pass(surface_indices, surface_count, dynamic_triangles_index,
                              nullptr, rasterizer_environment_shadow_draw, nullptr, nullptr);
        rasterizer_dynamic_triangles_delete(dynamic_triangles_index);
    }
}
