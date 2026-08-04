#include "headers/blam_data_globals.h"
extern void *hcex_stat_vbuf_create_semi_dyn(int size);

void *rasterizer_detail_objects_recreate_vbuf(void)
{
    void *result = hcex_stat_vbuf_create_semi_dyn(491520);
    local_d3d_vertex_buffer_0 = result;
    return result;
}
