/* rasterizer_dx9_shaders_unload_effects @ 0x836A0D30
   Releases every loaded effect in the shader table and nulls the slot. The
   binary bounds the loop with the adjacent shader_bin_handle global (123
   entries) and calls Release through the COM vtable (+8). */

#include "headers/rasterizer_dx9_shader_table.h"

void rasterizer_dx9_shaders_unload_effects(void)
{
    for ( int index = 0; index < 123; ++index )
    {
        if ( shader_table[index].effect )
        {
            ID3DXEffect_Release(shader_table[index].effect);  /* C boundary wrapper for opaque COM object */
            shader_table[index].effect = 0;
        }
    }
}
