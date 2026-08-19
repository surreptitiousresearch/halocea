/* tags_header_deregister_vertex_and_index_buffers @0x83753B48 */
#include "headers/cache_file_tags_header.h"
#include "headers/blam_data_globals.h"

extern void hcex_stat_vbuf_free(void *vbuf);

void tags_header_deregister_vertex_and_index_buffers(cache_file_tags_header *tags_header)
{
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
}
