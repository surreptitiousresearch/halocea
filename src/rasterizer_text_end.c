/* rasterizer_text_end @0x83814CD0 — close out the dynamic screen-geometry text pass opened by
 * rasterizer_text_begin: end the current effect pass and the effect itself (shader slot 122). If wireframe
 * debugging is on, the fill mode is restored to wireframe (it was forced solid for text). Only acts when
 * dynamic screen geometry drawing is enabled and the target is the main framebuffer. */

#include <stdint.h>
#include "headers/rasterizer_dx9_shader_table.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/rasterizer_window_begin_parameters.h"
#include "headers/d3d_render_boundary.h"
#include "headers/rasterizer_target.h"
#include "headers/blam_data_globals.h"
#include "headers/rasterizer_dx9_shader_index.h"


extern rasterizer_dx9_shader *rasterizer_shader_select(int16_t shader_index);
extern void D3DDevice_SetRenderState_FillMode(D3DDevice *device, unsigned int fill_mode);

void rasterizer_text_end(void)
{
    if (rasterizer_debug_options.draw_dynamic_screen_geometry && global_window_parameters.rasterizer_target == _rasterizer_target_render_primary)
    {
        rasterizer_dx9_shader *shader = rasterizer_shader_select(_dxshader_dyn_geom);
        ID3DXEffect_EndPass(shader->effect);
        ID3DXEffect_End(shader->effect);
    }

    if (rasterizer_debug_options.wireframe_enabled)
        D3DDevice_SetRenderState_FillMode(global_d3d_device, 0x25);
}
