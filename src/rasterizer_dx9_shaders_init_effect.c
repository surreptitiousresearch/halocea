/* rasterizer_dx9_shaders_init_effect @ 0x836A0D88
   Selects and validates a pixel-shader technique for shader_table[index], then
   binds the Texture0..3 parameter handles.

   Technique selection:
   - In safe mode (or when cf_ForceShader == 9997) it uses the "fallback"
     technique.
   - Otherwise it walks pixel-shader versions downward from the device's
     reported caps (ps_<major>_<minor>, minor 9..0, then next lower major),
     picking the first technique that both exists and validates, and sets it.
   - If none validate, it falls back to "TDefault_ps" or the first valid
     technique found by FindNextValidTechnique. */

#include <stdint.h>
#include "headers/rasterizer_dx9_shader_table.h"
#include "headers/rasterizer_engine_globals.h"  /* global_d3d_caps, cf_ForceShader */
#include "headers/blam_data_globals.h"

extern int sprintf_0(char *string, const char *format, ...);
extern long rasterizer_set_technique(ID3DXEffect *effect, unsigned int technique);

uint8_t rasterizer_dx9_shaders_init_effect(int index)
{
    unsigned int technique = 0;
    uint8_t succeeded;
    char name[144];

    if ( cf_SafeMode || cf_ForceShader == 9997 )
    {
        sprintf_0(name, "fallback");
        technique = ID3DXEffect_GetTechniqueByName(shader_table[index].effect, name);
        succeeded = technique != 0;
    }
    else
    {
        /* attest fix: disasm 836A0DD8 lbz PixelShaderVersion+2 (big-endian byte 2)
           = bits 15:8 — the D3DPS_VERSION major byte is >>8, not >>16 */
        int major = (global_d3d_caps.PixelShaderVersion >> 8) & 0xFF;
        int minor = global_d3d_caps.PixelShaderVersion & 0xFF;
        int validated = 0;

        while ( major >= 0 && !validated )
        {
            while ( minor >= 0 && !validated )
            {
                sprintf_0(name, "ps_%d_%d", major, minor);
                technique = ID3DXEffect_GetTechniqueByName(shader_table[index].effect, name);
                if ( technique )
                    validated = ID3DXEffect_ValidateTechnique(shader_table[index].effect, technique) >= 0;
                --minor;
            }
            --major;
            minor = 9;
        }

        if ( validated )
        {
            succeeded = rasterizer_set_technique(shader_table[index].effect, technique) >= 0;
        }
        else
        {
            sprintf_0(name, "TDefault_ps");
            technique = ID3DXEffect_GetTechniqueByName(shader_table[index].effect, name);
            int found = 1;
            if ( !technique )
            {
                found = ID3DXEffect_FindNextValidTechnique(shader_table[index].effect, 0, &technique) >= 0;
            }
            if ( !found
              || ID3DXEffect_ValidateTechnique(shader_table[index].effect, technique) < 0 )
                succeeded = 0;
            else
                succeeded = 1;
        }
    }

    if ( succeeded )
    {
        shader_table[index].texture[0] = ID3DXEffect_GetParameterByName(shader_table[index].effect, 0, "Texture0");
        shader_table[index].texture[1] = ID3DXEffect_GetParameterByName(shader_table[index].effect, 0, "Texture1");
        shader_table[index].texture[2] = ID3DXEffect_GetParameterByName(shader_table[index].effect, 0, "Texture2");
        shader_table[index].texture[3] = ID3DXEffect_GetParameterByName(shader_table[index].effect, 0, "Texture3");
    }
    return succeeded;
}
