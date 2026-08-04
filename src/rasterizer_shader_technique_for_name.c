/* rasterizer_shader_technique_for_name @ 0x836A1F40
   Finds the best pixel-shader-version technique for a base name on an effect:
   walks ps_<major>_<minor> downward from the device caps, returning the first
   "<name>_ps_M_m" technique that exists and validates (0 if none). */

#include "headers/rasterizer_dx9_shader_table.h"
#include "headers/rasterizer_engine_globals.h"  /* global_d3d_caps */

extern int sprintf_0(char *string, const char *format, ...);

unsigned int rasterizer_shader_technique_for_name(const char *name, ID3DXEffect *effect)
{
    unsigned int technique = 0;
    int validated = 0;
    /* deviation from prior reconstruction: binary reads the byte at
       PixelShaderVersion+2 (big-endian) = bits 15..8, i.e. >> 8, not >> 16
       (lbz @0x836A1F64 — D3DPS_VERSION packs major in bits 8..15) */
    int major = (global_d3d_caps.PixelShaderVersion >> 8) & 0xFF;
    int minor = global_d3d_caps.PixelShaderVersion & 0xFF;
    char full_name[192];

    while ( major >= 0 && !validated )
    {
        while ( minor >= 0 && !validated )
        {
            sprintf_0(full_name, "%s_ps_%d_%d", name, major, minor);
            technique = ID3DXEffect_GetTechniqueByName(effect, full_name);  /* C boundary wrapper */
            if ( technique )
                validated = ID3DXEffect_ValidateTechnique(effect, technique) >= 0;
            --minor;
        }
        --major;
        minor = 9;
    }
    return technique;
}
