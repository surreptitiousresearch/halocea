/* rasterizer_screen_effect_init_shaders @ 0x836861E8
   Resolves the eleven cinematic "video on/off" technique handles from shader
   114's effect. Fails if any technique is missing. */

#include <stdint.h>
#include "headers/rasterizer_video_techniques.h"
#include "headers/rasterizer_dx9_shader_index.h"

/* return attested uint8_t: callee normalizes clrlwi r3,r11,24 @0x8368640C and the
   sole caller re-normalizes clrlwi r11,r3,24 @0x836A2E7C (byte-normalization rule) */
uint8_t rasterizer_screen_effect_init_shaders(void)
{
    rasterizer_dx9_shader *shader = rasterizer_shader_select(_dxshader_screen_effect);

    hVideoOn = rasterizer_shader_technique_for_name("VideoOn", shader->effect);
    if ( !hVideoOn )
        return 0;
    hVideoOffNonConvolved = rasterizer_shader_technique_for_name("VideoOffNonConvolved", shader->effect);
    if ( !hVideoOffNonConvolved )
        return 0;
    hVideoOffConvolvedMask = rasterizer_shader_technique_for_name("VideoOffConvolvedMask", shader->effect);
    if ( !hVideoOffConvolvedMask )
        return 0;
    hVideoOffConvolveMaskThreeStage = rasterizer_shader_technique_for_name("VideoOffConvolvedMaskThreeStage", shader->effect);
    if ( !hVideoOffConvolveMaskThreeStage )
        return 0;
    hVideoOffConvolvedMaskFilterLightAndDesaturation = rasterizer_shader_technique_for_name("VideoOffConvolvedMaskFilterLightAndDesaturation", shader->effect);
    if ( !hVideoOffConvolvedMaskFilterLightAndDesaturation )
        return 0;
    hVideoOffConvolvedMaskFilterLight = rasterizer_shader_technique_for_name("VideoOffConvolvedMaskFilterLight", shader->effect);
    if ( !hVideoOffConvolvedMaskFilterLight )
        return 0;
    hVideoOffConvolvedMaskFilterDesaturation = rasterizer_shader_technique_for_name("VideoOffConvolvedMaskFilterDesaturation", shader->effect);
    if ( !hVideoOffConvolvedMaskFilterDesaturation )
        return 0;
    hVideoOffConvolved = rasterizer_shader_technique_for_name("VideoOffConvolved", shader->effect);
    if ( !hVideoOffConvolved )
        return 0;
    hVideoOffConvolvedFilterLightAndDesaturation = rasterizer_shader_technique_for_name("VideoOffConvolvedFilterLightAndDesaturation", shader->effect);
    if ( !hVideoOffConvolvedFilterLightAndDesaturation )
        return 0;
    hVideoOffConvolvedFilterLight = rasterizer_shader_technique_for_name("VideoOffConvolvedFilterLight", shader->effect);
    if ( !hVideoOffConvolvedFilterLight )
        return 0;
    hVideoOffConvolvedFilterDesaturation = rasterizer_shader_technique_for_name("VideoOffConvolvedFilterDesaturation", shader->effect);
    if ( !hVideoOffConvolvedFilterDesaturation )
        return 0;
    return 1;
}
