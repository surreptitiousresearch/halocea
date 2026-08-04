/* tag_register_vbufs @0x83754798 — free any previously registered HCEX static BSP/model vertex & index
 * buffers, then register the new tag header's vertex and index buffers. */
#include "headers/cache_file_tags_header.h"
#include "headers/blam_data_globals.h"
extern void hcex_stat_vbuf_free(void *vbuf);
extern void tags_header_register_vertex_and_index_buffers(cache_file_tags_header *tags_header);
void tag_register_vbufs(cache_file_tags_header *tags_header)
{
    if ( hcex_bsp_vbuf )
    {
        hcex_stat_vbuf_free(hcex_bsp_vbuf);
        hcex_bsp_vbuf = 0;
    }
    if ( hcex_models_vbuf )
    {
        hcex_stat_vbuf_free(hcex_models_vbuf);
        hcex_models_vbuf = 0;
    }
    if ( hcex_models_ibuf )
    {
        hcex_stat_vbuf_free(hcex_models_ibuf);
        hcex_models_ibuf = 0;
    }
    tags_header_register_vertex_and_index_buffers(tags_header);
}
