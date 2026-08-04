/* render_debug_fog_planes @0x83744CF0 — debug visualization: when `debug_fog_planes` is enabled and a
 * planar fog plane is active for the current cluster, draws the fog plane's boundary polygon as a ring of
 * edges: each edge as a white line at the plane itself, a black line offset by -planar_maximum_distance
 * along the plane normal, and two shaded lines connecting the two.
 *
 * The fog-plane record is structure_fog_plane (32 bytes): region_index @0, plane @4, vertices tag_block @20
 * (count @20, address @24). */

#include "headers/render_globals.h"
#include "headers/structure_bsp.h"
#include "headers/structure_cluster.h"
#include "headers/structure_fog_plane.h"
#include "headers/real_point3d.h"
#include "headers/real_argb_color.h"
#include "headers/blam_data_globals.h"
#include "headers/render_planar_fog_mode.h"


#include "headers/real_argb_color.h"
extern void rasterizer_debug_line(const real_point3d *p0, const real_point3d *p1, const real_argb_color *color);
extern void rasterizer_debug_line_shaded(const real_point3d *p0, const real_point3d *p1, const real_argb_color *color0, const real_argb_color *color1);

void render_debug_fog_planes(void)
{
    if ( !debug_fog_planes )
        return;

    if ( render.fog.planar_mode != _render_planar_fog_mode_normal || render.cluster_index == -1 )
        return;

    /* keep the original unsigned read: value feeds a 32*designator offset masked with 0xFFFE0 */
    unsigned __int16 cluster_fog_plane_designator = (unsigned __int16)
        ((structure_cluster *)global_structure_bsp->clusters.address)[render.cluster_index].fog_designator;
    /* (32*d) & 0xFFFE0 == (d & 0x7FFF)*32: mask strips the 0x8000 designator flag; 32 = sizeof(structure_fog_plane) */
    structure_fog_plane *fog_plane = &((structure_fog_plane *)global_structure_bsp->fog_planes.address)
        [cluster_fog_plane_designator & 0x7FFF];

    int vertex_count = fog_plane->vertices.count;
    if ( vertex_count <= 0 )
        return;

    const float *normal = fog_plane->plane.n.n;
    const real_point3d *vertices = (const real_point3d *)fog_plane->vertices.address;
    float offset = -render.fog.planar_maximum_distance;

    for ( int i = 0; i < vertex_count; i = (__int16)(i + 1) )
    {
        const real_point3d *p0 = &vertices[i];
        const real_point3d *p1 = &vertices[(__int16)((i + 1) % vertex_count)];

        real_point3d p0_offset, p1_offset;
        p0_offset.n[0] = normal[0] * offset + p0->n[0];
        p0_offset.n[1] = normal[1] * offset + p0->n[1];
        p0_offset.n[2] = normal[2] * offset + p0->n[2];
        p1_offset.n[0] = normal[0] * offset + p1->n[0];
        p1_offset.n[1] = normal[1] * offset + p1->n[1];
        p1_offset.n[2] = normal[2] * offset + p1->n[2];

        rasterizer_debug_line(p0, p1, global_real_argb_white);
        rasterizer_debug_line(&p0_offset, &p1_offset, global_real_argb_black);
        rasterizer_debug_line_shaded(p0, &p0_offset, global_real_argb_white, global_real_argb_black);
        rasterizer_debug_line_shaded(p1, &p1_offset, global_real_argb_white, global_real_argb_black);

        vertex_count = fog_plane->vertices.count;
    }
}
