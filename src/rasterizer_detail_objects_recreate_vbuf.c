#include "headers/blam_data_globals.h"
extern void *hcex_stat_vbuf_create_semi_dyn(int size);

/* DEVIATION: declared void — the vertex buffer is delivered through local_d3d_vertex_buffer_0;
 * the r3 left at blr (0x83712EFC) is the callee's residue and the sole caller ignores it. */
void rasterizer_detail_objects_recreate_vbuf(void)
{
    local_d3d_vertex_buffer_0 = hcex_stat_vbuf_create_semi_dyn(491520);
}
