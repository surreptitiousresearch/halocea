/* rasterizer_dx9_shaders_vdecl9_initialize @ 0x83723D58
   Creates the D3D vertex declarations for every vertex format and records the
   non-fixed-function vertex processing method (8) for each. Returns success
   only if every declaration was created.

   The decompiler renders each success test as a branchless fold of the
   E_OUTOFMEMORY HRESULT (0x8007000E); it reduces to "decl != NULL". */

#include <string.h>
#include "headers/rasterizer_dx9_shader_tables.h"
#include "headers/d3d_vertex_elements.h"

extern D3DVertexDeclaration *D3DDevice_CreateVertexDeclaration(D3DVERTEXELEMENT9 *elements);

unsigned int rasterizer_dx9_shaders_vdecl9_initialize(void)
{
    memset(vsd_table, 0, sizeof(vsd_table));

    unsigned int succeeded = 1;
    vsd_table[0].decl  = D3DDevice_CreateVertexDeclaration(d3dve_environment);
    succeeded &= (vsd_table[0].decl != 0);
    vsd_table[2].decl  = D3DDevice_CreateVertexDeclaration(d3dve_environment_lightmap);
    succeeded &= (vsd_table[2].decl != 0);
    vsd_table[4].decl  = D3DDevice_CreateVertexDeclaration(d3dve_model);
    succeeded &= (vsd_table[4].decl != 0);
    vsd_table[6].decl  = D3DDevice_CreateVertexDeclaration(d3dve_unlit);
    succeeded &= (vsd_table[6].decl != 0);
    vsd_table[7].decl  = D3DDevice_CreateVertexDeclaration(d3dve_unlit);
    succeeded &= (vsd_table[7].decl != 0);
    vsd_table[8].decl  = D3DDevice_CreateVertexDeclaration(d3dve_screen);
    succeeded &= (vsd_table[8].decl != 0);
    vsd_table[9].decl  = D3DDevice_CreateVertexDeclaration(d3dve_debug);
    succeeded &= (vsd_table[9].decl != 0);
    vsd_table[10].decl = D3DDevice_CreateVertexDeclaration(d3dve_decal);
    succeeded &= (vsd_table[10].decl != 0);
    vsd_table[11].decl = D3DDevice_CreateVertexDeclaration(d3dve_detail_object);
    succeeded &= (vsd_table[11].decl != 0);
    vsd_table[16].decl = D3DDevice_CreateVertexDeclaration(d3dve_unlit_zsprite);
    succeeded &= (vsd_table[16].decl != 0);
    vsd_table[17].decl = D3DDevice_CreateVertexDeclaration(d3dve_widget);

    vsd_table[7].vertex_processing_method  = 8;
    vsd_table[10].vertex_processing_method = 8;
    vsd_table[0].vertex_processing_method  = 8;
    vsd_table[4].vertex_processing_method  = 8;
    vsd_table[2].vertex_processing_method  = 8;
    vsd_table[6].vertex_processing_method  = 8;
    vsd_table[8].vertex_processing_method  = 8;
    vsd_table[9].vertex_processing_method  = 8;
    vsd_table[11].vertex_processing_method = 8;
    vsd_table[16].vertex_processing_method = 8;

    succeeded &= (vsd_table[17].decl != 0);
    return succeeded;
}
