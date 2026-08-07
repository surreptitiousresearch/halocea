/* set_chicago_shader @ 0x836A1548
   Selects (or compiles and caches) a fixed-function-emulation "chicago" pixel
   shader for the given texture-stage configuration. If a cached shader with the
   same cube flag exists it is bound directly; otherwise "chicago_shader.psh" is
   compiled (with USE_CUBE = is_cube), the bool stage constants are uploaded, the
   pixel shader is created and cached, optionally appended to the open blob, and
   bound.

   RELIABILITY: the Hex-Rays output for this function reports "local variable
   allocation has failed" and packs the three shader_stage_cfg arguments and the
   s4 operands into 64-bit registers with WORD2/HIDWORD/ROL bit-extraction.
   Attested against the raw disassembly 2026-07-28: the bool-constant block is
   82 ints (0x52) = three 25-int per-stage blocks (0x836A1590/15B0/15CC:
   +0/+0x64/+0xC8) followed by seven stage-4 one-hot selector flags
   (0x836A15E0..0x836A1648, cntlzw/extrwi == compares). The only observed
   callers pass zeroed configs. */

#include <string.h>
#include "headers/chicago_shaders.h"
#include "headers/d3dx_effect_boundary.h"      /* _D3DXMACRO, ID3DXBuffer, D3DXCompileShaderFromFileA */
#include "headers/win32_boundary.h"
#include "headers/rasterizer_engine_globals.h" /* global_d3d_device */

extern int sprintf_0(char *string, const char *format, ...);

void set_chicago_shader(int is_cube, shader_stage_cfg s1, shader_stage_cfg s2,
                        shader_stage_cfg s3, int s4_arg, int s4_op)
{
    int bool_constants[82]; /* 3 x 25 per-stage + 7 stage-4 one-hot = 0x52 bools */

    /* per-stage boolean constants for the fixed-function emulation */
    chicago_fill_const(&bool_constants[0], s1);
    chicago_fill_const(&bool_constants[25], s2);
    chicago_fill_const(&bool_constants[50], s3);
    /* stage-4 combiner one-hot selectors (disasm cntlzw/extrwi ==-decode) */
    bool_constants[75] = s4_arg == 0;
    bool_constants[76] = s4_arg == 1;
    bool_constants[77] = s4_arg == 2;
    bool_constants[78] = s4_op == 0;
    bool_constants[79] = s4_op == 1;
    bool_constants[80] = s4_op == 2;
    bool_constants[81] = s4_op == 3;
    D3DDevice_SetPixelShaderConstantB(global_d3d_device, 0, bool_constants, 0x52u);

    /* look for an already-compiled shader with the same cube flag */
    for ( int i = 0; i < chicago_shader_table_size; ++i )
    {
        if ( chicago_shader_table[i].is_cube == is_cube )
        {
            D3DDevice_SetPixelShader(global_d3d_device, chicago_shader_table[i].shader);
            return;
        }
    }

    /* not cached: compile a new chicago pixel shader */
    chicago_pixel_shader *entry = &chicago_shader_table[chicago_shader_table_size++];
    entry->is_cube = is_cube;
    entry->s1 = s1;
    entry->s2 = s2;
    entry->s3 = s3;
    entry->shader = 0;
    entry->s4_arg = s4_arg;
    entry->s4_op = s4_op;

    char cube_define[4];
    sprintf_0(cube_define, "%d", is_cube);
    _D3DXMACRO macros[13]; /* 104 bytes zeroed in the binary (stw + memset(+4,100)) */
    memset(macros, 0, sizeof(macros));
    macros[0].Name = "USE_CUBE";
    macros[0].Definition = cube_define;
    macros[1].Name = "xbox";
    macros[1].Definition = "1";

    char path[384];
    sprintf_0(path, "%s%s", "D:\\shaders\\fx\\", "chicago_shader.psh");

    ID3DXBuffer *compiled = 0;
    ID3DXBuffer *errors = 0;
    if ( D3DXCompileShaderFromFileA(path, macros, 0, "main", "ps_3_0", 0, &compiled, &errors, 0) < 0 )
    {
        if ( errors && errors->lpVtbl->GetBufferPointer(errors) )
            errors->lpVtbl->Release(errors);
        return;
    }

    const unsigned int *bytecode = compiled->lpVtbl->GetBufferPointer(compiled);
    entry->shader = D3DDevice_CreatePixelShader(bytecode);
    if ( shader_bin_handle != (void *)-1 )
    {
        unsigned int size = compiled->lpVtbl->GetBufferSize(compiled);
        unsigned int written;
        WriteFile(shader_bin_handle, &size, 4u, &written, 0);
        WriteFile(shader_bin_handle, bytecode, size, &written, 0);
    }
    compiled->lpVtbl->Release(compiled);
    D3DDevice_SetPixelShader(global_d3d_device, entry->shader);
}
