/* structure_bsp_header_register_vertex_buffers_new @0x83754630 — stream a structure BSP's vertex-buffer
 * block from the cache file into a freshly created static vertex buffer, then patch every lightmap
 * material's render-data record to point at the new GPU vertex buffer (and, where present, its separate
 * lightmap vertex stream). Blocks on the asynchronous read before patching. */

#include <stdint.h>
#include "headers/cache_file_structure_bsp_header.h"
#include "headers/structure_bsp.h"
#include "headers/structure_lightmap.h"
#include "headers/structure_material.h"
#include "headers/cache_file_read_request_params.h"
#include "headers/rasterizer_vertex_type.h"
#include "headers/blam_data_globals.h"

extern void *hcex_stat_vbuf_create(int size);
extern int   hcex_stat_vbuf_lock(void *buffer, int offset, int size, void **out_data);
extern void  hcex_stat_vbuf_unlock(void *buffer);
extern int16_t cache_file_read(int tag_index, uint32_t offset, int size, void *buffer, const cache_file_read_request_params *params, uint8_t blocking, uint8_t data_file);
extern void  Sleep(unsigned int milliseconds);

void structure_bsp_header_register_vertex_buffers_new(cache_file_structure_bsp_header *structure_bsp_header)
{
    int vertex_buffer_size = structure_bsp_header->vertex_buffer_size;
    char *vertex_data = nullptr;
    structure_bsp *structure_bsp = structure_bsp_header->structure_bsp;
    if (!vertex_buffer_size)
        return;

    hcex_bsp_vbuf = hcex_stat_vbuf_create(vertex_buffer_size);
    int vbuf_base = hcex_stat_vbuf_lock(hcex_bsp_vbuf, 0, structure_bsp_header->vertex_buffer_size,
                                        (void **)&vertex_data);

    volatile unsigned char finished = 0;
    cache_file_read_request_params params;
    params.finished_flag = &finished;
    params.finished_func = nullptr;
    params.userdata = nullptr;
    cache_file_read(-1, structure_bsp_header->vertex_buffer_file_ofs,
                    structure_bsp_header->vertex_buffer_size, vertex_data, &params, 1u, 0);
    while (!finished)
        Sleep(0);

    for (int lightmap_index = 0; lightmap_index < structure_bsp->lightmaps.count;
         lightmap_index = (int16_t)(lightmap_index + 1))
    {
        structure_lightmap *lightmap = &((structure_lightmap *)structure_bsp->lightmaps.address)[lightmap_index];
        int material_count = lightmap->materials.count;
        for (int material_index = 0; material_index < material_count;
             material_index = (int16_t)(material_index + 1))
        {
            structure_material *material = &((structure_material *)lightmap->materials.address)[material_index];
            int vertex_offset = material->vertices.offset;
            int lightmap_vertex_count = material->lightmap_vertices.count;

            material->vertices.type = _rasterizer_vertex_type_environment_uncompressed;
            material->lightmap_vertices.hardware_format = 0;
            material->uncompressed_vertex_data.address = &vertex_data[vertex_offset];
            material->vertices.offset = vertex_offset + vbuf_base;
            material->vertices.hardware_format = hcex_bsp_vbuf;
            if (lightmap_vertex_count)
            {
                int lightmap_vertex_offset = material->lightmap_vertices.offset;
                material->lightmap_vertices.type = _rasterizer_vertex_type_environment_lightmap_uncompressed;
                material->lightmap_vertices.hardware_format = hcex_bsp_vbuf;
                material->lightmap_vertices.offset = vbuf_base + lightmap_vertex_offset;
            }
        }
    }
    hcex_stat_vbuf_unlock(hcex_bsp_vbuf);
}
