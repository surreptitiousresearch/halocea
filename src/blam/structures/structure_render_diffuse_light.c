/* structure_render_diffuse_light @0x837C6F20 — render the environment surfaces lit by one diffuse scene
 * light. When a cluster index list is supplied the affected surfaces and their dynamic triangles are built on
 * the spot (and freed afterward); otherwise the frame's cached environment surface set is reused. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/render_globals.h"
#include "headers/structure_render_globals.h"
#include "headers/structure_render_pass.h"

#include "headers/real_rectangle3d.h"
#include "headers/real_plane3d.h"
extern int16_t structure_visibility_build_surfaces(int *surface_indices, int16_t maximum_count, const real_point3d *bounding_sphere_center, float bounding_sphere_radius, const real_rectangle3d *bounding_box, int16_t bounding_surface_count, const real_plane3d *bounding_surfaces, int16_t cluster_count, int16_t *cluster_indices);
extern int structure_render_build_dynamic_triangles(int *surface_indices, int16_t surface_count, unsigned int *surface_flags);
extern void rasterizer_environment_diffuse_light_begin(int light_index);
extern void rasterizer_environment_diffuse_light_draw(const struct shader *shader, int16_t shader_permutation_index, int dynamic_triangle_buffer_index, int first_triangle_index, int triangle_count, const struct vertex_buffer *vertex_buffer);
extern void rasterizer_environment_diffuse_light_end(void);
extern void rasterizer_dynamic_triangles_delete(int dynamic_triangle_buffer_index);

/* The decompiler's float-shadow miscount split the trailing (cluster_count, cluster_indices) pair into a
 * phantom count plus an "a6" flag; the real signature matches the caller (lights_render_diffuse). */
void structure_render_diffuse_light(int rasterizer_light_index, const real_point3d *bounding_sphere_center, float bounding_sphere_radius, int16_t cluster_count, int16_t *cluster_indices)
{
    int local_surface_buffer[4096];
    int *environment_surface_indices = local_surface_buffer;
    int16_t environment_surface_count;
    int environment_triangles_index;

    if (cluster_indices)
    {
        environment_surface_count = structure_visibility_build_surfaces(local_surface_buffer, 4096,
            bounding_sphere_center, bounding_sphere_radius, 0, 0, 0, cluster_count, cluster_indices);
        environment_triangles_index = structure_render_build_dynamic_triangles(local_surface_buffer,
            environment_surface_count, 0);
    }
    else
    {
        environment_surface_indices = render.environment_surface_indices;
        environment_triangles_index = structure_render_globals.environment_triangles_index;
        environment_surface_count = render.environment_surface_count;
    }

    if (environment_triangles_index != -1)
    {
        rasterizer_environment_diffuse_light_begin(rasterizer_light_index);
        structure_render_pass(environment_surface_indices, environment_surface_count,
                              environment_triangles_index, 0,
                              rasterizer_environment_diffuse_light_draw, 0, 0);
        if (cluster_indices)
            rasterizer_dynamic_triangles_delete(environment_triangles_index);
        rasterizer_environment_diffuse_light_end();
    }
}
