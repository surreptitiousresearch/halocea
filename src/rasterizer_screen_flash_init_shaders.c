/* rasterizer_screen_flash_init_shaders @ 0x83686FC0
   Resolves the six screen-flash technique handles from shader 115's effect.
   Fails if any technique is missing. */

#include <stdint.h>
#include "headers/rasterizer_flash_techniques.h"
#include "headers/rasterizer_dx9_shader_index.h"

/* return attested uint8_t: callee normalizes clrlwi r3,r11,24 @0x836870F4 and the
   sole caller re-normalizes clrlwi r11,r3,24 @0x836A2EA0 (byte-normalization rule) */
uint8_t rasterizer_screen_flash_init_shaders(void)
{
    rasterizer_dx9_shader *shader = rasterizer_shader_select(_dxshader_screen_flash);

    hFlashLighten = rasterizer_shader_technique_for_name("FlashLighten", shader->effect);
    if ( !hFlashLighten )
        return 0;
    hFlashDarken = rasterizer_shader_technique_for_name("FlashDarken", shader->effect);
    if ( !hFlashDarken )
        return 0;
    hFlashMax = rasterizer_shader_technique_for_name("FlashMax", shader->effect);
    if ( !hFlashMax )
        return 0;
    hFlashMin = rasterizer_shader_technique_for_name("FlashMin", shader->effect);
    if ( !hFlashMin )
        return 0;
    hFlashInvert = rasterizer_shader_technique_for_name("FlashInvert", shader->effect);
    if ( !hFlashInvert )
        return 0;
    hFlashTint = rasterizer_shader_technique_for_name("FlashTint", shader->effect);
    if ( !hFlashTint )
        return 0;
    return 1;
}
