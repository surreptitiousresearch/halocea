/* set_decal_shader @ 0x836A18C8
   Selects the decal pixel shader for a framebuffer blend function. Sets a
   one-hot boolean constant vector selecting the blend path (0 -> slot 3;
   1 or 5 -> slot 1; 2 -> slot 2; otherwise -> slot 0), uploads it, then binds
   the (lazily compiled & cached) decal shader. */

#include <stdint.h>
#include "headers/chicago_shaders.h"
#include "headers/shader_framebuffer_blend_function.h"
#include "headers/d3dx_effect_boundary.h"      /* D3DXCompileShaderFromFileA, ID3DXBuffer */
#include "headers/win32_boundary.h"
#include "headers/rasterizer_engine_globals.h" /* global_d3d_device */

extern int sprintf_0(char *string, const char *format, ...);

void set_decal_shader(int16_t local_framebuffer_blend_function)
{
    int blend_flags[4] = { 0, 0, 0, 0 };

    if ( (unsigned int)local_framebuffer_blend_function <= _shader_framebuffer_blend_function_max )
    {
        switch ( local_framebuffer_blend_function )
        {
            case _shader_framebuffer_blend_function_alpha_blend:
                blend_flags[3] = 1; break;
            case _shader_framebuffer_blend_function_multiply:
            case _shader_framebuffer_blend_function_min:
                blend_flags[1] = 1; break;
            case _shader_framebuffer_blend_function_double_multiply:
                blend_flags[2] = 1; break;
            default:
                blend_flags[0] = 1; break;
        }
    }
    D3DDevice_SetPixelShaderConstantB(global_d3d_device, 0, blend_flags, 4u);

    if ( decal_shader )
    {
        D3DDevice_SetPixelShader(global_d3d_device, decal_shader);
        return;
    }

    char path[304];
    sprintf_0(path, "%s%s", "D:\\shaders\\fx\\", "decal_shader.psh");

    ID3DXBuffer *compiled = 0;
    ID3DXBuffer *errors = 0;
    if ( D3DXCompileShaderFromFileA(path, 0, 0, "main", "ps_3_0", 0, &compiled, &errors, 0) >= 0 )
    {
        const unsigned int *bytecode = compiled->lpVtbl->GetBufferPointer(compiled);
        decal_shader = D3DDevice_CreatePixelShader(bytecode);
        if ( shader_bin_handle != (void *)-1 )
        {
            unsigned int size = compiled->lpVtbl->GetBufferSize(compiled);
            unsigned int written;
            WriteFile(shader_bin_handle, &size, 4u, &written, 0);
            WriteFile(shader_bin_handle, bytecode, size, &written, 0);
        }
        compiled->lpVtbl->Release(compiled);
        D3DDevice_SetPixelShader(global_d3d_device, decal_shader);
        return;
    }

    if ( errors && errors->lpVtbl->GetBufferPointer(errors) )
        errors->lpVtbl->Release(errors);
}
