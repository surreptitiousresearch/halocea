/* rasterizer_shaders_dispose @0x836A1DF0 — tear down the DX9 vertex-declaration, vertex-shader, and
 * pixel-shader tables in order.
 * DEVIATION: the decompiler chained each callee's return value into the next call's argument
 * (v0 -> vshader9_dispose(v0) -> dispose(v1)); all three sub-dispose functions have no resolved DB
 * prototype and match the codebase's other `*_dispose` no-op/void-release convention, so they are called
 * here as plain void() functions rather than threading a meaningless garbage return value between them. */

typedef struct ID3DXEffectPool ID3DXEffectPool;
extern void rasterizer_dx9_shaders_vdecl9_dispose(void);
extern void rasterizer_dx9_shaders_vshader9_dispose(void);
extern void rasterizer_dx9_shaders_dispose(void);

void rasterizer_shaders_dispose(void)
{
    rasterizer_dx9_shaders_vdecl9_dispose();
    rasterizer_dx9_shaders_vshader9_dispose();
    rasterizer_dx9_shaders_dispose();
}
