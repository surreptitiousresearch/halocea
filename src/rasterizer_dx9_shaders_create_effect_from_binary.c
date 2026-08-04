/* rasterizer_dx9_shaders_create_effect_from_binary @ 0x836A1050
   Creates one effect for shader_table[shaderIndex] from a precompiled binary
   blob via D3DXCreateEffect. On failure it raises an error dialog. */

#include <stdint.h>
#include "headers/rasterizer_dx9_shader_table.h"
#include "headers/d3dx_boundary.h"             /* ID3DXBuffer */
#include "headers/rasterizer_engine_globals.h" /* global_d3d_device */
#include "headers/blam_data_globals.h"

extern int DisplayErrorDialog(int Error, int ErrorURL, int ForceExit);

uint8_t rasterizer_dx9_shaders_create_effect_from_binary(unsigned int shaderIndex,
                                                         uint8_t *buffer, unsigned int bufferSize)
{
    rasterizer_dx9_shader *entry = &shader_table[shaderIndex];
    entry->effect = 0;

    ID3DXBuffer *errors = 0;
    long hr = D3DXCreateEffect(global_d3d_device, buffer, bufferSize, global_effect_macros,
                               0, 0, global_effect_pool, &entry->effect, &errors);
    if ( hr < 0 )
    {
        AppendText = "D:\\maps\\fx.bin";
        DisplayErrorDialog(105, 126, 1);
    }
    /* attest fix: disasm 836A10D0-E8 releases the errors buffer (vtbl slot 8)
       when non-NULL — this was missing from the reconstruction */
    if ( errors )
        errors->lpVtbl->Release(errors);
    return hr >= 0;
}
