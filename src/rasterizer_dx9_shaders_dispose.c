/* rasterizer_dx9_shaders_dispose @0x836A1C84 — tear down the DX9 pixel-shader table. Disposes the
 * Chicago (multitexture) shaders, frees the constant-handle arrays held by several disjoint runs of
 * shader_table entries, unloads the compiled effects, and releases the shared effect pool. */

#include "headers/rasterizer_dx9_shader_table.h"
#include <stddef.h>   /* NULL */

extern void dispose_chicago_shaders(void);
extern void dlFree(void *ptr);
extern void rasterizer_dx9_shaders_unload_effects(void);

static void free_constants_range(int first, int last)
{
    for (int i = first; i <= last; ++i)
    {
        if (shader_table[i].constants)
        {
            dlFree(shader_table[i].constants);
            shader_table[i].constants = NULL;
        }
    }
}

void rasterizer_dx9_shaders_dispose(void)
{
    dispose_chicago_shaders();

    free_constants_range(116, 121);
    free_constants_range(32, 34);
    free_constants_range(37, 39);
    free_constants_range(106, 106);
    free_constants_range(107, 107);
    free_constants_range(108, 108);
    free_constants_range(0, 3);
    free_constants_range(114, 114);
    free_constants_range(40, 43);

    rasterizer_dx9_shaders_unload_effects();

    if (global_effect_pool)
    {
        global_effect_pool->lpVtbl->Release(global_effect_pool);  /* opaque COM: call through vtbl */
        global_effect_pool = NULL;
    }
}
