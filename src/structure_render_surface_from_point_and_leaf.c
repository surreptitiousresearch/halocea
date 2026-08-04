/* structure_render_surface_from_point_and_leaf @0x83744690 — given a point known to lie in a render
 * leaf and the collision plane it hit, find the render surface (triangle) under the point. Walks the
 * leaf's surface references, skips those whose collision bsp3d node plane does not match, then does a
 * 3D point-in-triangle test on each candidate. On a hit, fills *lightmap_index, *material_index,
 * *surface_index and the barycentric (s,t), and returns 1; returns 0 if no surface contains the point.
 *
 * Leaf record (16 bytes): first surface reference @+12, reference count @+10. Surface reference
 * (8 bytes): surface_index @0, collision bsp3d node @+4. Lightmap record (32 bytes): structure_material
 * block pointer @+24. */

#include <stdint.h>
#include "headers/structure_bsp.h"
#include "headers/structure_leaf.h"
#include "headers/structure_lightmap.h"
#include "headers/structure_material.h"
#include "headers/structure_surface.h"
#include "headers/structure_surface_reference.h"
#include "headers/collision_bsp.h"
#include "headers/environment_vertex_compressed.h"
#include "headers/environment_vertex_uncompressed.h"
#include "headers/real_point3d.h"
#include "headers/rasterizer_vertex_type.h"

extern void structure_bsp_find_material_for_surface(structure_bsp *structure, int surface_index, int16_t *lightmap_index, int16_t *material_index);
extern void environment_vertex_compressed_get_point(const environment_vertex_compressed *vertex, real_point3d *point);
extern uint8_t point_in_triangle3d(const real_point3d *point, const real_point3d *p0, const real_point3d *p1, const real_point3d *p2, float *s, float *t);

uint8_t structure_render_surface_from_point_and_leaf(const real_point3d *point, int leaf_index,
        int plane_index, int16_t *lightmap_index, int16_t *material_index, int *surface_index,
        float *s, float *t)
{
    structure_bsp *structure = global_structure_bsp;
    const structure_leaf *leaf = &((const structure_leaf *)structure->leaves.address)[leaf_index];
    int first_reference = leaf->first_surface_reference_index;
    int reference_count = leaf->surface_reference_count;
    int reference;

    for ( reference = first_reference; reference < first_reference + reference_count; ++reference )
    {
        const structure_surface_reference *surface_reference =
            &((const structure_surface_reference *)structure->surface_references.address)[reference];
        int bsp3d_node = surface_reference->bsp3d_node_index;
        const collision_bsp *collision = (const collision_bsp *)structure->collision_bsp.address;
        const int *bsp3d_nodes = (const int *)collision->bsp3d.nodes.address;
        const structure_surface *surface;
        const structure_lightmap *lightmap;
        structure_material *material;
        real_point3d tri_vertex0, tri_vertex1, tri_vertex2;
        int inside;

        /* bsp3d node = {int plane_index, int back_child, int front_child} (12 bytes); plane_index is [0] */
        if ( bsp3d_node == -1 || bsp3d_nodes[3 * bsp3d_node] != plane_index )
            continue;

        surface = (const structure_surface *)structure->surfaces.address + surface_reference->surface_index;
        structure_bsp_find_material_for_surface(structure, surface_reference->surface_index,
                lightmap_index, material_index);

        lightmap = &((const structure_lightmap *)structure->lightmaps.address)[*lightmap_index];
        material = &((structure_material *)lightmap->materials.address)[*material_index];

        if ( material->vertices.type == _rasterizer_vertex_type_environment_compressed )
        {
            const environment_vertex_compressed *verts =
                (const environment_vertex_compressed *)material->compressed_vertex_data.address;
            environment_vertex_compressed_get_point(&verts[surface->vertex_indices[0]], &tri_vertex0);
            environment_vertex_compressed_get_point(&verts[surface->vertex_indices[1]], &tri_vertex1);
            environment_vertex_compressed_get_point(&verts[surface->vertex_indices[2]], &tri_vertex2);
            inside = point_in_triangle3d(point, &tri_vertex0, &tri_vertex1, &tri_vertex2, s, t);
        }
        else if ( material->vertices.type == _rasterizer_vertex_type_environment_uncompressed || material->vertices.type == _rasterizer_vertex_type_environment_uncompressed_ff )
        {
            const environment_vertex_uncompressed *verts =
                (const environment_vertex_uncompressed *)material->uncompressed_vertex_data.address;
            tri_vertex0 = verts[surface->vertex_indices[0]].position;
            tri_vertex1 = verts[surface->vertex_indices[1]].position;
            tri_vertex2 = verts[surface->vertex_indices[2]].position;
            inside = point_in_triangle3d(point, &tri_vertex0, &tri_vertex1, &tri_vertex2, s, t);
        }
        else
            continue;

        if ( inside )
        {
            *surface_index = surface_reference->surface_index;
            return 1;
        }
    }
    return 0;
}
