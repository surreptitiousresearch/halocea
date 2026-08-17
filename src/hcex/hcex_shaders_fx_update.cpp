/* hcex_shaders_fx_update @0x836A2DE0 */
#include <stdint.h>
#include "../headers/ID3DXEffectPool.h"

extern "C" void rasterizer_dx9_shaders_dispose(void);
extern "C" uint8_t rasterizer_dx9_shaders_initialize(void);

extern "C" int hcex_shaders_fx_update(void)
{
    rasterizer_dx9_shaders_dispose();
    return rasterizer_dx9_shaders_initialize();
}
