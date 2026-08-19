/* rasterizer_shaders_update @ 0x836A2E08
   Rebuilds all shader resources. Short-circuit chain: any failing step makes
   the whole update fail. On reaching the vertex shaders it disposes and
   re-initializes the DX9 shader set before the per-effect shader inits. */

#include <stdint.h>

extern uint8_t rasterizer_dx9_shaders_vdecl9_update(void);
extern uint8_t rasterizer_dx9_shaders_vshader9_update(void);
typedef struct ID3DXEffectPool ID3DXEffectPool;
extern void rasterizer_dx9_shaders_dispose(void);
extern uint8_t rasterizer_dx9_shaders_initialize(void);
extern uint8_t rasterizer_screen_effect_init_shaders(void);
extern uint8_t rasterizer_screen_flash_init_shaders(void);
extern uint8_t rasterizer_model_init_shaders(void);

uint8_t rasterizer_shaders_update(void)
{
    if ( !rasterizer_dx9_shaders_vdecl9_update() || !rasterizer_dx9_shaders_vshader9_update() )
        return 0;

    rasterizer_dx9_shaders_dispose();
    if ( !rasterizer_dx9_shaders_initialize() )
        return 0;
    if ( !rasterizer_screen_effect_init_shaders() )
        return 0;
    if ( !rasterizer_screen_flash_init_shaders() )
        return 0;
    if ( !rasterizer_model_init_shaders() )
        return 0;
    return 1;
}
