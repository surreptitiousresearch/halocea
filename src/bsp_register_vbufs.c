/* bsp_register_vbufs @0x8375484C — registers a newly loaded structure BSP's vertex buffers with the
 * rasterizer, first freeing any previously registered HCEX BSP vertex buffer. */

#include "headers/cache_file_structure_bsp_header.h"
#include "headers/blam_data_globals.h"

extern void hcex_stat_vbuf_free(void *vbuf);
extern void structure_bsp_header_register_vertex_buffers_new(cache_file_structure_bsp_header *structure_bsp_header);

void bsp_register_vbufs(cache_file_structure_bsp_header *structure_bsp_header)
{
    if (hcex_bsp_vbuf)
    {
        hcex_stat_vbuf_free(hcex_bsp_vbuf);
        hcex_bsp_vbuf = nullptr;
    }
    structure_bsp_header_register_vertex_buffers_new(structure_bsp_header);
}
