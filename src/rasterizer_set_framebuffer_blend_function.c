/* rasterizer_set_framebuffer_blend_function @0x8369C358 — program the D3D source/dest blend factors and
 * blend op for one of the engine's framebuffer blend functions, looking the three render-state values up in
 * the blend tables. As a hardware workaround, when min/max blend ops are broken on this device and the
 * requested function is one of the two min/max modes (5 or 6), it falls back to a plain additive blend. */

#include <stdint.h>
#include "headers/d3d_render_boundary.h"
#include "headers/shader_framebuffer_blend_function.h"
#include "headers/blam_data_globals.h"

extern const unsigned int srcblend_table[];
extern const unsigned int destblend_table[];
extern const int blendop_table[];

extern void D3DDevice_SetRenderState_SrcBlend(D3DDevice *device, unsigned int blend);
extern void D3DDevice_SetRenderState_DestBlend(D3DDevice *device, unsigned int blend);
extern void D3DDevice_SetRenderState_BlendOp(D3DDevice *device, unsigned int op);

void rasterizer_set_framebuffer_blend_function(int16_t framebuffer_blend_function)
{
    char is_min_max = (framebuffer_blend_function == _shader_framebuffer_blend_function_min
                    || framebuffer_blend_function == _shader_framebuffer_blend_function_max);

    if (cf_MinMaxBlendOpIsBroken && is_min_max)
    {
        D3DDevice_SetRenderState_SrcBlend(global_d3d_device, 1);
        D3DDevice_SetRenderState_DestBlend(global_d3d_device, 1);
        D3DDevice_SetRenderState_BlendOp(global_d3d_device, 0);
    }
    else
    {
        D3DDevice_SetRenderState_SrcBlend(global_d3d_device, srcblend_table[framebuffer_blend_function]);
        D3DDevice_SetRenderState_DestBlend(global_d3d_device, destblend_table[framebuffer_blend_function]);
        D3DDevice_SetRenderState_BlendOp(global_d3d_device, blendop_table[framebuffer_blend_function]);
    }
}
