/* rasterizer_dx9_shaders_vshader9_update @ 0x83723C70
   Rebuilds the DX9 vertex shaders: dispose then re-initialize. */

#include <stdint.h>

extern void rasterizer_dx9_shaders_vshader9_dispose(void);
extern uint8_t rasterizer_dx9_shaders_vshader9_initialize(void);

uint8_t rasterizer_dx9_shaders_vshader9_update(void)
{
    rasterizer_dx9_shaders_vshader9_dispose();
    return rasterizer_dx9_shaders_vshader9_initialize();
}
