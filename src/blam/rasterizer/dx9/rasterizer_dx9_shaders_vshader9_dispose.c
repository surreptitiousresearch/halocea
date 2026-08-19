/* rasterizer_dx9_shaders_vshader9_dispose @ 0x837237F0
   Releases every compiled vertex shader in vsf_table and nulls the slot.

   The binary bounds the loop with the address of the adjacent global
   keyboard_auxbutton_mapping (the symbol following vsf_table); that span is
   exactly the 65-entry table, so we iterate the table by its element count. */

#include "headers/rasterizer_dx9_shader_tables.h"

void rasterizer_dx9_shaders_vshader9_dispose(void)
{
    for ( int i = 0; i < 65; ++i )
    {
        if ( vsf_table[i].shader )
        {
            /* COM upcast: a vertex shader is-a D3DResource; Release lives on the base interface. */
            D3DResource_Release((D3DResource *)vsf_table[i].shader);
            vsf_table[i].shader = 0;
        }
    }
}
