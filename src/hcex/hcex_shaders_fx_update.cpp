#include <stdint.h>
#include "../headers/ID3DXEffectPool.h"

extern void rasterizer_dx9_shaders_dispose(void);
extern uint8_t rasterizer_dx9_shaders_initialize(void);

extern "C" int hcex_shaders_fx_update(void)
{
    rasterizer_dx9_shaders_dispose();
    return rasterizer_dx9_shaders_initialize();
}
