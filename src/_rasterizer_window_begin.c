/* _rasterizer_window_begin @0x8369D8F8 — platform rasterizer window-begin implementation (called through
 * the rasterizer_window_begin thunk). Resets default render state, latches the window parameters, opens each
 * per-window rasterizer subsystem, configures fog/stencil/visibility, and sets the render target (clearing
 * to the packed atmospheric fog color unless drawing in wireframe debug mode). Finally applies the fill mode.
 *
 * The memory-pool / dynamic-geometry / transparent-geometry / lights begin calls are independent no-arg
 * calls (threaded-r3 artifact). */

#include <stdint.h>
#include "headers/rasterizer_window_begin_parameters.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/d3d_render_boundary.h"
#include "headers/rasterizer_target.h"
#include "headers/blam_data_globals.h"
#include "headers/rasterizer_drawing_mode.h"


extern void _rasterizer_set_default_state(void);
extern void *memcpy(void *dst, const void *src, unsigned int size);
extern void rasterizer_memory_pool_begin(void);
extern void rasterizer_dynamic_geometry_begin(void);
extern void rasterizer_transparent_geometry_begin(void);
extern void rasterizer_lights_begin(void);
extern void rasterizer_water_set_visibility_for_window(uint8_t visibility);
extern void rasterizer_active_camouflage_set_visibility(uint8_t visibility);
extern void rasterizer_set_stencil_mode(int16_t stencil_mode);
extern void rasterizer_window_set_fog(const render_fog *fog);
extern void rasterizer_set_target(int16_t target, int16_t mipmap_index, uint32_t background_color, uint8_t clear, uint8_t zbuffer);
extern void rasterizer_set_frustum_z(float z_near, float z_far);
extern void D3DDevice_SetRenderState_FillMode(D3DDevice *device, unsigned int fill_mode);

void _rasterizer_window_begin(const rasterizer_window_begin_parameters *parameters)
{
    _rasterizer_set_default_state();
    memcpy(&global_window_parameters, parameters, sizeof(global_window_parameters));

    rasterizer_memory_pool_begin();
    rasterizer_dynamic_geometry_begin();
    rasterizer_transparent_geometry_begin();
    rasterizer_lights_begin();

    rasterizer_water_set_visibility_for_window(0);
    rasterizer_active_camouflage_set_visibility(0);
    rasterizer_set_stencil_mode(0);
    rasterizer_window_set_fog(&parameters->fog);

    unsigned int clear_color;
    if (rasterizer_debug_options.drawing_mode == _rasterizer_drawing_mode_overdraw)
    {
        clear_color = 0;
    }
    else
    {
        /* recovered: (const float *)(global_window_parameters.fog + 4) -> fog.atmospheric_color */
        const real_rgb_color *atmospheric_color = &global_window_parameters.fog.atmospheric_color;
        unsigned int red   = (uint8_t)(atmospheric_color->n[0] * 255.0f);
        unsigned int green = (uint8_t)(atmospheric_color->n[1] * 255.0f);
        unsigned int blue  = (uint8_t)(atmospheric_color->n[2] * 255.0f);
        clear_color = (red << 16) | (green << 8) | blue;
    }

    int16_t rasterizer_target = parameters->rasterizer_target;
    if (rasterizer_target == _rasterizer_target_render_primary || rasterizer_target == _rasterizer_target_render_secondary)
        rasterizer_set_target(rasterizer_target, 0, clear_color, parameters->suppress_clear == 0, 1u);

    rasterizer_set_frustum_z(-1.0f, -1.0f);
    D3DDevice_SetRenderState_FillMode(global_d3d_device,
                                      rasterizer_debug_options.wireframe_enabled == 0 ? 0 : 0x25);
}
