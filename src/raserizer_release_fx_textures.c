/* raserizer_release_fx_textures @0x836A1BE8 — (DB name kept verbatim, including the misspelling)
 * clear every bound texture on all loaded pixel-shader effects. For each populated shader_table
 * entry, walk its 4 texture parameter handles and, for any non-null handle, unbind the texture
 * by setting it to NULL on the effect.
 * DEVIATION: declared void — the SetTexture HRESULT left in r3 at blr is loop residue, and neither
 * of the two binary callers consumes r3 (disasm 0x836A1C44 bctrl, 0x836A1C74). */

#include "headers/rasterizer_dx9_shader_table.h"

/* ID3DXEffect_SetTexture — D3DX effect boundary wrapper (obj->Method rewritten to free fn). */
extern long ID3DXEffect_SetTexture(ID3DXEffect *effect, unsigned int parameter, void *texture);

void raserizer_release_fx_textures(void)
{
    int shader_index;
    int texture_slot;

    for ( shader_index = 0; shader_index < 123; shader_index = (short)(shader_index + 1) )
    {
        if ( !shader_table[shader_index].effect )
            continue;

        for ( texture_slot = 0; texture_slot < 4; texture_slot = (short)(texture_slot + 1) )
        {
            unsigned int texture_handle = shader_table[shader_index].texture[texture_slot];
            if ( texture_handle )
                ID3DXEffect_SetTexture(shader_table[shader_index].effect, texture_handle, 0);
        }
    }
}
