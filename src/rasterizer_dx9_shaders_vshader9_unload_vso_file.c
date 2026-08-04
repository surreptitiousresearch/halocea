/* rasterizer_dx9_shaders_vshader9_unload_vso_file @0x837237A0 — releases and clears the D3D vertex
 * shader at the given vsf_table slot, if one is loaded. */

#include "headers/rasterizer_dx9_shader_tables.h"

extern unsigned int D3DResource_Release(D3DResource *resource);

void rasterizer_dx9_shaders_vshader9_unload_vso_file(int index)
{
    D3DVertexShader *shader = vsf_table[index].shader;
    if (shader)
    {
        D3DResource_Release((D3DResource *)shader);
        vsf_table[index].shader = nullptr;
    }
}
