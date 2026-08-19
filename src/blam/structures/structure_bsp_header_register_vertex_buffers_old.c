/* structure_bsp_header_register_vertex_buffers_old @0x83753BA8 — original (pre-HCEX) vertex-buffer
 * registration path: for every lightmap material, build a hardware vertex_buffer in place at the material
 * record (primary at +176/+180, from the raw geometry data at +228; lightmap/secondary at +196/+200, from
 * the geometry data offset past the primary vertices, 56 bytes/vertex) via rasterizer_vertex_buffer_new.
 * Counterpart of structure_bsp_header_register_vertex_buffers_new.c, which shares the same material-record
 * field layout (+176/+180/+192/+196/+200/+212/+228) but streams into HCEX's custom async vertex buffer
 * instead of calling the rasterizer directly. */

#include <stdint.h>
#include "headers/cache_file_structure_bsp_header.h"
#include "headers/structure_bsp.h"
#include "headers/structure_lightmap.h"
#include "headers/structure_material.h"
#include "headers/vertex_buffer.h"
#include "headers/rasterizer_vertex_type.h"
#include "headers/environment_vertex_uncompressed.h"

extern int16_t rasterizer_geometry_get_vertex_size(int16_t type);
extern uint8_t rasterizer_vertex_buffer_new(vertex_buffer *vertex_buffer, int16_t type, int count, const void *vertices, const void *vertices2, int buffer_size);

void structure_bsp_header_register_vertex_buffers_old(cache_file_structure_bsp_header *structure_bsp_header)
{
    structure_bsp *structure_bsp = structure_bsp_header->structure_bsp;

    for ( int lightmap_index = 0; lightmap_index < structure_bsp->lightmaps.count; lightmap_index = (int16_t)(lightmap_index + 1) )
    {
        structure_lightmap *lightmap = (structure_lightmap *)structure_bsp->lightmaps.address + lightmap_index;
        int material_count = lightmap->materials.count;

        for ( int material_index = 0; material_index < material_count; material_index = (int16_t)(material_index + 1) )
        {
            structure_material *material = (structure_material *)lightmap->materials.address + material_index;

            vertex_buffer *primary_vertices = &material->vertices;
            vertex_buffer *lightmap_vertices = &material->lightmap_vertices;

            lightmap_vertices->hardware_format = 0;
            primary_vertices->hardware_format = 0;

            int vertex_count = primary_vertices->count;
            char *vertex_data = (char *)material->uncompressed_vertex_data.address;
            /* the folded 56 was sizeof(environment_vertex_uncompressed); the lightmap vertices
             * follow the render vertices in the same stream */
            char *lightmap_vertex_data =
                (char *)((environment_vertex_uncompressed *)vertex_data + vertex_count);

            int primary_vertex_size = rasterizer_geometry_get_vertex_size(_rasterizer_vertex_type_environment_uncompressed);
            rasterizer_vertex_buffer_new(primary_vertices, _rasterizer_vertex_type_environment_uncompressed, (int16_t)vertex_count,
                vertex_data, 0, primary_vertex_size * (int16_t)vertex_count);

            int lightmap_vertex_count = lightmap_vertices->count; /* recovered: *(int *)(material + 200) -> lightmap_vertices.count */
            if ( lightmap_vertex_count )
            {
                int lightmap_vertex_size = rasterizer_geometry_get_vertex_size(_rasterizer_vertex_type_environment_lightmap_uncompressed);
                rasterizer_vertex_buffer_new(lightmap_vertices, _rasterizer_vertex_type_environment_lightmap_uncompressed, (int16_t)lightmap_vertex_count,
                    lightmap_vertex_data, 0, lightmap_vertex_size * (int16_t)lightmap_vertex_count);
            }
        }
    }
}
