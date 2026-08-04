/* rasterizer_dx9_shaders_write_effect @ 0x836A1128
   Dev tooling: compiles one pixel-shader effect from "D:\shaders\fx\<name>.fx"
   and appends it to the open blob as [u32 size][bytecode]. On failure it builds
   a diagnostic message (with the compiler error text) and raises a dialog.

   The compiled-effect buffer is an ID3DXBuffer accessed by the binary through
   raw COM vtable slots (+12 GetBufferPointer, +16 GetBufferSize, +8 Release);
   reproduced here via the named methods. */

#include <stdint.h>
#include "headers/rasterizer_dx9_shader_table.h"
#include "headers/d3dx_effect_boundary.h"
#include "headers/win32_boundary.h"
#include "headers/blam_data_globals.h"

extern int sprintf_0(char *string, const char *format, ...);
extern char *strncat(char *dst, const char *src, unsigned int count);
extern int DisplayErrorDialog(int Error, int ErrorURL, int ForceExit);

uint8_t rasterizer_dx9_shaders_write_effect(unsigned int shaderIndex, void *fileHandle)
{
    ID3DXBuffer *errors = 0;
    ID3DXBuffer *compiled = 0;
    ID3DXEffectCompiler *compiler = 0;
    unsigned int bytecode_size = 0;
    const void *bytecode = 0;
    char path[272];
    char message[2112];

    sprintf_0(path, "%s%s.fx", "D:\\shaders\\fx\\", shader_table[shaderIndex].filename);

    uint8_t succeeded = D3DXCreateEffectCompilerFromFileA(path, global_effect_macros, 0, 0,
                                                          &compiler, &errors) >= 0;
    if ( succeeded )
    {
        succeeded = ID3DXEffectCompiler_CompileEffect(compiler, 0, &compiled, &errors) >= 0;
        if ( succeeded )
        {
            bytecode = compiled->lpVtbl->GetBufferPointer(compiled);
            bytecode_size = compiled->lpVtbl->GetBufferSize(compiled);
            succeeded = bytecode != 0;
        }
    }

    if ( succeeded )
    {
        unsigned int written;
        WriteFile(fileHandle, &bytecode_size, 4u, &written, 0);
        WriteFile(fileHandle, bytecode, bytecode_size, &written, 0);
    }
    else
    {
        message[0] = 0;
        strncat(message, "Failed to compile effect:", 0x800u);
        strncat(message, shader_table[shaderIndex].filename, 0x800u);
        strncat(message, "\n", 0x800u);
        if ( errors )
        {
            const char *error_text = (const char *)errors->lpVtbl->GetBufferPointer(errors);
            if ( error_text )
                strncat(message, error_text, 0x800u);
        }
        message[2047] = 0;
        AppendText = "D:\\maps\\fx.bin";
        /* PUN kept: disasm 836A12E4 passes &message in r4 — DisplayErrorDialog's
           ErrorURL slot doubles as a message pointer when Error == -1 (attested
           wave-1 sig keeps it int) */
        DisplayErrorDialog(-1, (int)message, 1);
    }

    if ( compiled )
    {
        compiled->lpVtbl->Release(compiled);
        compiled = 0;
    }
    if ( errors )
    {
        errors->lpVtbl->Release(errors);
        errors = 0;
    }
    if ( compiler )
        ID3DXEffectCompiler_Release(compiler);
    return succeeded;
}
