#include "headers/blam_data_globals.h"
/* _rasterizer_decals_dispose @0x83710FAC — internal decal-system shutdown: free the shared D3D vertex
 * buffer used to draw decals. (Public guard wrapper: rasterizer_decals_dispose.) */

extern void dlFree(void *ptr);

void _rasterizer_decals_dispose(void)
{
    if ( local_d3d_vertex_buffer )
        dlFree(local_d3d_vertex_buffer);
}
