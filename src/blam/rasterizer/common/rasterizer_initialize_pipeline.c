/* rasterizer_initialize_pipeline @0x8369BA28 — run the per-subsystem "initialize pipeline" passes in
 * fixed order (environment lightmaps, glass, models, environment diffuse textures, environment
 * diffuse lights, water).
 *
 * DEVIATION: the decompiler threaded each callee's r3 return value into the next call and gave the
 * function an int parameter/return; every callee is actually void(void), so the threading is PPC
 * register-reuse noise. Reproduced as a plain sequence of void calls. */

extern void rasterizer_environment_lightmaps_initialize_pipeline(void);
extern void rasterizer_glass_initialize_pipeline(void);
extern void rasterizer_models_initialize_pipeline(void);
extern void rasterizer_environment_diffuse_textures_initialize_pipeline(void);
extern void rasterizer_environment_diffuse_lights_initialize_pipeline(void);
extern void rasterizer_water_initialize_pipeline(void);

void rasterizer_initialize_pipeline(void)
{
    rasterizer_environment_lightmaps_initialize_pipeline();
    rasterizer_glass_initialize_pipeline();
    rasterizer_models_initialize_pipeline();
    rasterizer_environment_diffuse_textures_initialize_pipeline();
    rasterizer_environment_diffuse_lights_initialize_pipeline();
    rasterizer_water_initialize_pipeline();
}
