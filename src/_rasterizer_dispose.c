#include "headers/d3d_boundary.h"
#include "headers/rasterizer_globals.h"
#include "headers/rasterizer_engine_globals.h"

extern void rasterizer_memory_pool_dispose(void);
extern void rasterizer_dynamic_geometry_dispose(void);
extern void rasterizer_transparent_geometry_dispose(void);
extern void rasterizer_debug_dispose(void);
extern void rasterizer_frame_statistics_dispose(void);
extern void rasterizer_text_cache_dispose(void);
extern void rasterizer_detail_objects_dispose(void);
extern void rasterizer_environment_fog_screen_dispose(void);
extern void rasterizer_shaders_dispose(void);
extern void rasterizer_render_targets_dispose(void);
extern void rasterizer_occulsion_test_dispose(void);
extern void D3DDevice_Release(D3DDevice *device);

void _rasterizer_dispose(void)
{
    rasterizer_memory_pool_dispose();
    rasterizer_dynamic_geometry_dispose();
    rasterizer_transparent_geometry_dispose();
    rasterizer_debug_dispose();
    rasterizer_frame_statistics_dispose();
    rasterizer_text_cache_dispose();
    rasterizer_detail_objects_dispose();
    rasterizer_environment_fog_screen_dispose();
    rasterizer_shaders_dispose();
    rasterizer_render_targets_dispose();
    rasterizer_occulsion_test_dispose();
    if (rasterizer_globals.default_white_hardware_format)
    {
        D3DResource_Release((D3DResource *)rasterizer_globals.default_white_hardware_format);
        rasterizer_globals.default_white_hardware_format = 0;
    }
    if (rasterizer_globals.default_2d_hardware_format)
    {
        D3DResource_Release((D3DResource *)rasterizer_globals.default_2d_hardware_format);
        rasterizer_globals.default_2d_hardware_format = 0;
    }
    if (rasterizer_globals.default_3d_hardware_format)
    {
        D3DResource_Release((D3DResource *)rasterizer_globals.default_3d_hardware_format);
        rasterizer_globals.default_3d_hardware_format = 0;
    }
    if (rasterizer_globals.default_cm_hardware_format)
    {
        D3DResource_Release((D3DResource *)rasterizer_globals.default_cm_hardware_format);
        rasterizer_globals.default_cm_hardware_format = 0;
    }
    if (global_d3d_device)
        D3DDevice_Release(global_d3d_device);
    global_d3d_device = 0;
    global_d3d = 0;
}
