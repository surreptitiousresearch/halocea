#include <stdint.h>
#include "headers/ID3DXEffectPool.h"
extern void rasterizer_dx9_shaders_dispose(void);
extern uint8_t rasterizer_dx9_shaders_initialize(void);

int rasterizer_dx9_shaders_update(void)
{
    rasterizer_dx9_shaders_dispose();
    return rasterizer_dx9_shaders_initialize();
}
