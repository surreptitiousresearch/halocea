/* rasterizer_dx9_shaders_load_effect @0x836A1360 — compile a single pixel-shader
 * effect from its "D:\shaders\fx\<name>.fx" source into the shared effect pool.
 * Returns non-zero on success. */

#include "headers/rasterizer_dx9_shader_table.h"
#include "headers/d3dx_boundary.h"             /* ID3DXBuffer */
#include "headers/rasterizer_engine_globals.h" /* global_d3d_device */

extern int sprintf_0(char *string, const char *format, ...);

int rasterizer_dx9_shaders_load_effect(unsigned int shaderIndex)
{
    rasterizer_dx9_shader *entry;
    ID3DXBuffer *errors = 0;
    char path[272];
    int created;

    shader_table[shaderIndex].effect = 0;
    entry = &shader_table[shaderIndex];

    sprintf_0(path, "%s%s.fx", "D:\\shaders\\fx\\", shader_table[shaderIndex].filename);
    created = D3DXCreateEffectFromFileA(global_d3d_device, path, global_effect_macros,
                                        0, 0, global_effect_pool, &entry->effect, &errors) >= 0;
    if ( errors )
        errors->lpVtbl->Release(errors);  /* opaque COM: call through vtbl (matches sibling files) */

    return created;
}
