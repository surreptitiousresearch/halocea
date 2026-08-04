/* rasterizer_dx9_shaders_vdecl9_dispose @ 0x83723F58
   Releases every vertex declaration in vsd_table, then zeroes the whole table.
   The binary bounds the release loop with the address of the adjacent global
   global_rasterizer_data (the symbol following vsd_table) — the 18-entry span. */

#include "headers/rasterizer_dx9_shader_tables.h"

extern void *memset(void *dst, int value, unsigned int size);

void rasterizer_dx9_shaders_vdecl9_dispose(void)
{
    for ( int i = 0; i < 18; ++i )
    {
        if ( vsd_table[i].decl )
            D3DResource_Release((D3DResource *)vsd_table[i].decl);
    }
    memset(vsd_table, 0, sizeof(vsd_table));
}
