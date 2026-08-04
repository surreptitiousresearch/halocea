/* rasterizer_dx9_shaders_vdecl9_get @0x83723FC0 — return the D3D vertex declaration at the given slot of the
 * vertex-declaration table. */

#include "headers/rasterizer_dx9_shader_tables.h"

D3DVertexDeclaration *rasterizer_dx9_shaders_vdecl9_get(unsigned int index)
{
    return vsd_table[index].decl;
}
