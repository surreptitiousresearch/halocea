/* rasterizer_dx9_shaders_vshader9_write_effect @ 0x83723550
   Compiles one vertex shader from "D:\shaders\vsh\<name>.vsh" and appends it to
   the open binary blob as [u32 size][bytecode]. On compile failure it builds a
   diagnostic message (including the D3DX error text) and raises an error
   dialog. Dev tooling path. */

#include <stdint.h>
#include "headers/rasterizer_dx9_shader_tables.h"
#include "headers/d3dx_boundary.h"
#include "headers/win32_boundary.h"
#include "headers/blam_data_globals.h"

extern int sprintf_0(char *string, const char *format, ...);
extern char *strncat(char *dst, const char *src, unsigned int count);
extern int DisplayErrorDialog(int Error, int ErrorURL, int ForceExit);

uint8_t rasterizer_dx9_shaders_vshader9_write_effect(int shaderIndex, void *fileHandle)
{
    ID3DXBuffer *compiled = 0;
    ID3DXBuffer *errors = 0;
    char path[272];
    char message[2056];

    sprintf_0(path, "%s%s.vsh", "D:\\shaders\\vsh\\", vsf_table[shaderIndex].filename);

    /* ppConstantTable is explicitly NULL: disasm 837235A8 stw r30(=0) into the
       9th (stack) argument slot */
    if ( D3DXCompileShaderFromFileA(path, 0, 0, "main", "vs_3_0", 0,
                                    &compiled, &errors, 0) < 0 )
    {
        message[0] = 0;
        strncat(message, "Failed to assemble shader: ", 0x800u);
        strncat(message, path, 0x800u);
        strncat(message, "\n", 0x800u);
        if ( errors )
        {
            const char *error_text = (const char *)errors->lpVtbl->GetBufferPointer(errors);
            if ( error_text )
                strncat(message, error_text, 0x800u);
            errors->lpVtbl->Release(errors);
        }
        message[2047] = 0;
        AppendText = "D:\\maps\\vsh.bin";
        /* PUN kept: disasm 837236F0 passes &message in r4 — DisplayErrorDialog's
           ErrorURL slot doubles as a message pointer when Error == -1 (attested
           wave-1 sig keeps it int) */
        DisplayErrorDialog(-1, (int)message, 1);
        return 0;
    }

    const void *bytecode = compiled->lpVtbl->GetBufferPointer(compiled);
    unsigned int bytecode_size = compiled->lpVtbl->GetBufferSize(compiled);
    unsigned int written;
    WriteFile(fileHandle, &bytecode_size, 4u, &written, 0);
    WriteFile(fileHandle, bytecode, bytecode_size, &written, 0);
    compiled->lpVtbl->Release(compiled);
    return 1;
}
