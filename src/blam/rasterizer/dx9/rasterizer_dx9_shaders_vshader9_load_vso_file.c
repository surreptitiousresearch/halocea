/* rasterizer_dx9_shaders_vshader9_load_vso_file @ 0x83723460
   Compiles one vertex shader (vs_3_0) from "D:\shaders\vsh\<name>.vsh" and
   installs the created shader into vsf_table[index]. On compile failure it
   raises an error dialog. */

#include <stdint.h>
#include "headers/rasterizer_dx9_shader_tables.h"
#include "headers/d3dx_boundary.h"
#include "headers/blam_data_globals.h"

extern int sprintf_0(char *string, const char *format, ...);
extern int DisplayErrorDialog(int Error, int ErrorURL, int ForceExit);

uint8_t rasterizer_dx9_shaders_vshader9_load_vso_file(int index)
{
    ID3DXBuffer *compiled = 0;
    ID3DXBuffer *errors = 0;
    char path[304];

    sprintf_0(path, "%s%s.vsh", "D:\\shaders\\vsh\\", vsf_table[index].filename);

    /* ppConstantTable is explicitly NULL: disasm 837234AC stw r31(=0) into the
       9th (stack) argument slot */
    long hr = D3DXCompileShaderFromFileA(path, 0, 0, "main", "vs_3_0", 0,
                                         &compiled, &errors, 0);
    uint8_t succeeded = hr >= 0;
    if ( hr < 0 )
    {
        AppendText = path;
        DisplayErrorDialog(137, 126, 1);
    }
    else
    {
        const unsigned int *function = (const unsigned int *)compiled->lpVtbl->GetBufferPointer(compiled);
        vsf_table[index].shader = D3DDevice_CreateVertexShader(function);
        compiled->lpVtbl->Release(compiled);
    }
    return succeeded;
}
