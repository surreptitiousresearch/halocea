/* _rasterizer_initialize @ 0x8369BA60
   Core rasterizer bring-up: configures rasterizer_globals, acquires the D3D
   device and caps, then initializes every rasterizer subsystem in order. The
   block of subsystem inits from rasterizer_shaders_initialize through
   rasterizer_occulsion_test_initialize is combined with short-circuit AND so a
   single failure makes the function report failure (return value `succeeded`).

   Decompiler notes:
   - The pipeline-init calls are rendered as `v6 = f(v5)`; that argument is the
     decompiler threading r3 between no-arg calls. They take no parameters.
   - The original signature shows (int,int,int,__int64) — these are unused
     register artifacts; the function takes no real arguments. */

#include <stdint.h>
#include "headers/rasterizer_globals.h"
#include "headers/rasterizer_engine_globals.h"
#include "headers/rasterizer_limits.h"

/* D3D / hcex boundary + accessor helpers */
extern D3DDevice *hcex_get_global_d3d_device(void);
extern Direct3D *hcex_get_global_d3d(void);

/* Subsystem pipeline inits (return value ignored) */
extern void rasterizer_environment_lightmaps_initialize_pipeline(void);
extern void rasterizer_glass_initialize_pipeline(void);
extern void rasterizer_models_initialize_pipeline(void);
extern void rasterizer_environment_diffuse_textures_initialize_pipeline(void);
extern void rasterizer_environment_diffuse_lights_initialize_pipeline(void);
extern void rasterizer_water_initialize_pipeline(void);
extern void rasterizer_filthy_bitmap_defaults_initialize(void);

/* Subsystem inits returning success flags */
extern uint8_t rasterizer_shaders_initialize(void);
extern uint8_t rasterizer_memory_pool_initialize(void);
extern uint8_t rasterizer_dynamic_geometry_initialize(void);
extern uint8_t rasterizer_transparent_geometry_initialize(void);
extern uint8_t rasterizer_debug_initialize(void);
extern uint8_t rasterizer_frame_statistics_initialize(void);
extern uint8_t rasterizer_text_cache_initialize(void);
extern uint8_t rasterizer_detail_objects_initialize(void);
extern uint8_t rasterizer_environment_fog_screen_initialize(void);
extern uint8_t rasterizer_render_targets_initialize(void);
extern uint8_t rasterizer_occulsion_test_initialize(void);

/* Unconditional tail inits */
typedef struct lruv_cache lruv_cache;   /* texture_cache_new returns it; only forward decl needed here */
extern void rasterizer_screen_effects_initialize(void);
extern lruv_cache *texture_cache_new(void);
extern void rasterizer_debug_begin(void);
extern uint8_t rasterizer_windows_begin(void);
extern void rasterizer_windows_end(void);

int _rasterizer_initialize(void)
{
    use_fullscreen = 1;
    rasterizer_globals.alpha_render_targets_disabled = 0;
    rasterizer_globals.use_cheap_active_camo = 0;
    rasterizer_globals.screen_bounds.y0 = 0;       // n[0]
    rasterizer_globals.screen_bounds.x0 = 0;       // n[1]
    rasterizer_globals.frame_bounds.y0 = 0;
    rasterizer_globals.frame_bounds.x0 = 0;
    rasterizer_globals.render_targets_disabled = 0;
    rasterizer_globals.screen_bounds.x1 = hcex_screen_width;   // n[3]
    rasterizer_globals.screen_bounds.y1 = hcex_screen_height;  // n[2]
    global_devicetype = D3DDEVTYPE_HAL;
    rasterizer_globals.frame_index = 1;
    rasterizer_globals.frame_bounds.x1 = hcex_screen_width;
    rasterizer_globals.frame_bounds.y1 = hcex_screen_height;

    global_d3d_device = hcex_get_global_d3d_device();
    global_d3d = hcex_get_global_d3d();
    Direct3D_GetDeviceCaps(0, global_devicetype, &global_d3d_caps);

    cf_ForceShader = 0;
    if ( (unsigned short)global_d3d_caps.PixelShaderVersion > 0x200u )
        global_d3d_caps.PixelShaderVersion = 0xFFFF02FF; // -65024: clamp to PS 2.0 token

    rasterizer_globals.maximum_nodes_per_model = RASTERIZER_MAXIMUM_NODES_PER_MODEL;
    device_is_lost = 0;
    flip_status = 0;
    rasterizer_globals.max_simultaneous_textures =
        global_d3d_caps.MaxSimultaneousTextures >= 4 ? 4 : 2;

    rasterizer_environment_lightmaps_initialize_pipeline();
    rasterizer_glass_initialize_pipeline();
    rasterizer_models_initialize_pipeline();
    rasterizer_environment_diffuse_textures_initialize_pipeline();
    rasterizer_environment_diffuse_lights_initialize_pipeline();
    rasterizer_water_initialize_pipeline();
    rasterizer_filthy_bitmap_defaults_initialize();

    unsigned char succeeded =
        rasterizer_shaders_initialize()
        && rasterizer_memory_pool_initialize()
        && rasterizer_dynamic_geometry_initialize()
        && rasterizer_transparent_geometry_initialize()
        && rasterizer_debug_initialize()
        && rasterizer_frame_statistics_initialize()
        && rasterizer_text_cache_initialize()
        && rasterizer_detail_objects_initialize()
        && rasterizer_environment_fog_screen_initialize()
        && rasterizer_render_targets_initialize()
        && rasterizer_occulsion_test_initialize();

    rasterizer_screen_effects_initialize();
    texture_cache_new();
    rasterizer_debug_begin();
    if ( rasterizer_windows_begin() )
        rasterizer_windows_end();

    rasterizer_globals.active = 1;
    rasterizer_initialized = 1;
    return succeeded;
}
