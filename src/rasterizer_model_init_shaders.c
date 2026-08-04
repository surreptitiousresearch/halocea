/* rasterizer_model_init_shaders @ 0x8378A920
   Resolves every model pixel-shader technique handle from the six model shaders
   (116..121): environment-no-mask, self-illumination, change-color,
   multipurpose, reflection, and the no-mask reflection set. Each technique name
   is built from a prefix and a mask-name suffix. Any missing technique fails
   the whole init.

   The 12-entry "no-mask" tables index the mask names as i<6 ? i : i+6 (mask
   names 0..5 and 12..17), matching the binary; the 24-entry tables use one
   technique per mask name. */

#include <stdint.h>
#include "headers/rasterizer_model_techniques.h"
#include "headers/rasterizer_dx9_shader_index.h"

extern int sprintf_0(char *string, const char *format, ...);

static int resolve_full(rasterizer_dx9_shader *shader, const char *prefix,
                        unsigned int *table, int count)
{
    char name[192];
    for ( int i = 0; i < count; ++i )
    {
        sprintf_0(name, "%s%s", prefix, mask_technique_names[i]);
        table[i] = rasterizer_shader_technique_for_name(name, shader->effect);
        if ( !table[i] )
            return 0;
    }
    return 1;
}

static int resolve_nomask(rasterizer_dx9_shader *shader, const char *prefix,
                          unsigned int *table)
{
    char name[192];
    for ( int i = 0; i < 12; ++i )
    {
        int mask_index = i < 6 ? i : i + 6;
        sprintf_0(name, "%s%s", prefix, mask_technique_names[mask_index]);
        table[i] = rasterizer_shader_technique_for_name(name, shader->effect);
        if ( !table[i] )
            return 0;
    }
    return 1;
}

/* return attested uint8_t: callee normalizes clrlwi r3,r11,24 @0x8378AC7C/0x8378ACF8 and
   the sole caller re-normalizes clrlwi r11,r3,24 @0x836A2EC4 (byte-normalization rule) */
uint8_t rasterizer_model_init_shaders(void)
{
    rasterizer_dx9_shader *shader = rasterizer_shader_select(_dxshader_model_environment);
    if ( !shader || !resolve_nomask(shader, "EnvironmentNo", hModelEnvironmentNoMaskTechniques) )
        return 0;

    shader = rasterizer_shader_select(_dxshader_model_self_illumination);
    if ( !shader || !resolve_full(shader, "SelfIllumination", hModelSelfIlluminationTechniques, 24) )
        return 0;

    shader = rasterizer_shader_select(_dxshader_model_change_color);
    if ( !shader || !resolve_full(shader, "ChangeColor", hModelChangeColorTechniques, 24) )
        return 0;

    shader = rasterizer_shader_select(_dxshader_model_multipurpose);
    if ( !shader || !resolve_full(shader, "Multipurpose", hModelMultipurposeMaskTechniques, 24) )
        return 0;

    shader = rasterizer_shader_select(_dxshader_model_reflection);
    if ( !shader || !resolve_full(shader, "Reflection", hModelReflectionTechniques, 24) )
        return 0;

    shader = rasterizer_shader_select(_dxshader_model_normal);
    if ( !shader || !resolve_nomask(shader, "No", hModelNoMaskTechniques) )
        return 0;

    return 1;
}
