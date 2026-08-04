#include "headers/blam_data_globals.h"
extern void hcex_stat_vbuf_free(void *vbuf);

void structure_bsp_header_deregister_vertex_buffers(void)
{
    if (hcex_bsp_vbuf)
    {
        hcex_stat_vbuf_free(hcex_bsp_vbuf);
        hcex_bsp_vbuf = 0;
    }
}
