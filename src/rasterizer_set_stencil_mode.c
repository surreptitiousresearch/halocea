/* rasterizer_set_stencil_mode @0x8369D220 — program the D3D stencil unit for one of the renderer's stencil
 * modes (0=off, 1..5 = the various mask/test configurations used for portals, mirrors, decals, etc.). When
 * the global stencil-mask debug option is disabled the mode is forced to 0 (stencil off). The last applied
 * mode is cached in previous_stencil_mode so redundant device calls are skipped. */

#include <stdint.h>
#include "headers/d3d_render_boundary.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/rasterizer_stencil_mode.h"
#include "headers/blam_data_globals.h"

/* Stencil op codes below are _D3DSTENCILOP (KEEP=0, REPLACE=2); compare-func codes are _D3DCMPFUNC
 * (NEVER=0, EQUAL=2, NOTEQUAL=5, ALWAYS=7). The D3DDevice_SetRenderState_* wrappers take plain int,
 * so the values are annotated inline rather than retyped through the D3D enum headers. */


extern void D3DDevice_SetRenderState_StencilEnable(D3DDevice *device, int enable);
extern void D3DDevice_SetRenderState_StencilFail(D3DDevice *device, int op);
extern void D3DDevice_SetRenderState_StencilZFail(D3DDevice *device, int op);
extern void D3DDevice_SetRenderState_StencilPass(D3DDevice *device, int op);
extern void D3DDevice_SetRenderState_StencilFunc(D3DDevice *device, unsigned int func);
extern void D3DDevice_SetRenderState_StencilRef(D3DDevice *device, int ref);
extern void D3DDevice_SetRenderState_StencilMask(D3DDevice *device, int mask);
extern void D3DDevice_SetRenderState_StencilWriteMask(D3DDevice *device, int mask);

void rasterizer_set_stencil_mode(int16_t stencil_mode)
{
    /* mask == 0xFFFF when the debug option is on, 0 when off → forces mode 0 in that case. */
    int16_t mask = rasterizer_debug_options.stencil_mask_enabled ? -1 : 0;
    int16_t mode = mask & stencil_mode;

    if ((uint16_t)mode == (uint16_t)previous_stencil_mode)
        return;

    if ((uint16_t)mode <= _rasterizer_stencil_mode_reject_alpha_tested_decal)
    {
        if (mode == _rasterizer_stencil_mode_write)
        {
            D3DDevice_SetRenderState_StencilEnable(global_d3d_device, 1);
            D3DDevice_SetRenderState_StencilFail(global_d3d_device, 0);    /* D3DSTENCILOP_KEEP */
            D3DDevice_SetRenderState_StencilZFail(global_d3d_device, 0);   /* D3DSTENCILOP_KEEP */
            D3DDevice_SetRenderState_StencilPass(global_d3d_device, 2);    /* D3DSTENCILOP_REPLACE */
            D3DDevice_SetRenderState_StencilFunc(global_d3d_device, 7);    /* D3DCMP_ALWAYS */
            D3DDevice_SetRenderState_StencilRef(global_d3d_device, 1);
            D3DDevice_SetRenderState_StencilMask(global_d3d_device, 1);
            D3DDevice_SetRenderState_StencilWriteMask(global_d3d_device, 1);
            previous_stencil_mode = mode;
            return;
        }

        int stencil_mask;
        if (mode == _rasterizer_stencil_mode_reject)
        {
            D3DDevice_SetRenderState_StencilEnable(global_d3d_device, 1);
            D3DDevice_SetRenderState_StencilFail(global_d3d_device, 0);    /* D3DSTENCILOP_KEEP */
            D3DDevice_SetRenderState_StencilZFail(global_d3d_device, 0);   /* D3DSTENCILOP_KEEP */
            D3DDevice_SetRenderState_StencilPass(global_d3d_device, 0);    /* D3DSTENCILOP_KEEP */
            D3DDevice_SetRenderState_StencilFunc(global_d3d_device, 2);    /* D3DCMP_EQUAL */
            D3DDevice_SetRenderState_StencilRef(global_d3d_device, 0);
            stencil_mask = 1;
        }
        else if (mode == _rasterizer_stencil_mode_reject_invert && mode != _rasterizer_stencil_mode_none)
        {
            D3DDevice_SetRenderState_StencilEnable(global_d3d_device, 1);
            D3DDevice_SetRenderState_StencilFail(global_d3d_device, 0);    /* D3DSTENCILOP_KEEP */
            D3DDevice_SetRenderState_StencilZFail(global_d3d_device, 0);   /* D3DSTENCILOP_KEEP */
            D3DDevice_SetRenderState_StencilPass(global_d3d_device, 0);    /* D3DSTENCILOP_KEEP */
            D3DDevice_SetRenderState_StencilFunc(global_d3d_device, 5);    /* D3DCMP_NOTEQUAL */
            D3DDevice_SetRenderState_StencilRef(global_d3d_device, 0);
            stencil_mask = 1;
        }
        else
        {
            if (mode == _rasterizer_stencil_mode_write_alpha_tested_decal && mode != _rasterizer_stencil_mode_none)
            {
                D3DDevice_SetRenderState_StencilEnable(global_d3d_device, 1);
                D3DDevice_SetRenderState_StencilFail(global_d3d_device, 0);   /* D3DSTENCILOP_KEEP */
                D3DDevice_SetRenderState_StencilZFail(global_d3d_device, 0);  /* D3DSTENCILOP_KEEP */
                D3DDevice_SetRenderState_StencilPass(global_d3d_device, 2);   /* D3DSTENCILOP_REPLACE */
                D3DDevice_SetRenderState_StencilFunc(global_d3d_device, 2);   /* D3DCMP_EQUAL */
                D3DDevice_SetRenderState_StencilRef(global_d3d_device, 2);
                D3DDevice_SetRenderState_StencilMask(global_d3d_device, 1);
                D3DDevice_SetRenderState_StencilWriteMask(global_d3d_device, 2);
                previous_stencil_mode = mode;
                return;
            }
            if (mode == _rasterizer_stencil_mode_none)
            {
                D3DDevice_SetRenderState_StencilEnable(global_d3d_device, 0);
                previous_stencil_mode = mode;
                return;
            }
            /* mode == _rasterizer_stencil_mode_reject_alpha_tested_decal */
            D3DDevice_SetRenderState_StencilEnable(global_d3d_device, 1);
            D3DDevice_SetRenderState_StencilFail(global_d3d_device, 0);    /* D3DSTENCILOP_KEEP */
            D3DDevice_SetRenderState_StencilZFail(global_d3d_device, 0);   /* D3DSTENCILOP_KEEP */
            D3DDevice_SetRenderState_StencilPass(global_d3d_device, 0);    /* D3DSTENCILOP_KEEP */
            D3DDevice_SetRenderState_StencilFunc(global_d3d_device, 2);    /* D3DCMP_EQUAL */
            D3DDevice_SetRenderState_StencilRef(global_d3d_device, 0);
            stencil_mask = 3;
        }

        D3DDevice_SetRenderState_StencilMask(global_d3d_device, stencil_mask);
        D3DDevice_SetRenderState_StencilWriteMask(global_d3d_device, 0);
    }

    previous_stencil_mode = mode;
}
