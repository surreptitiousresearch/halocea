/* hcex_init_render_targets @ 0x823EF8E0
   Allocates the hardware render-target textures (slots 1..8) and wires their
   D3D texture/surface into global_render_targets. Slots 1 and 2 reuse the
   manager's full-screen 8888 targets; the rest are created on demand sized to
   their render target. Slot 9's surface is the driver depth buffer (ref-added).

   Asserts (compiled out unless IGNORE_STRONG_ASSERT is clear) require an
   8888/X8R8G8B8 format and a successful Create. */

#include "../headers/vid_boundary.h"        /* hcxe_global_render_targets_tex, txmD3D_TEX */
#include "../headers/txm_boundary.h"        /* txmManager, Find/Add, ETADDR_CLAMP */
#include "../headers/txm_d3d_tex.h"         /* txmD3D_TEX__GetD3DTex */
#include "../headers/d3d_driver.h"          /* d3dDriver->pDepthBuffer */
#include "../headers/rasterizer_render_target.h" /* global_render_targets */

extern int sprintf_0(char *string, const char *format, ...);
extern int IGNORE_STRONG_ASSERT;
extern const char empty_string[]; /* shared "" @0x8200155A */
extern void STRONG_ASSERT_DUMMY_Crash(void *ctx, const char *expr, const char *file, int line, const char *info);

extern "C" unsigned int hcex_init_render_targets(void)
{
    for ( int i = 1; i < 9; ++i )
    {
        txmD3D_TEX *tex;
        if ( i == 1 )
        {
            tex = (txmD3D_TEX *)txmMANAGER__Find(txmManager, "__FULL_8888_00__");
            tex->Create(0, 0, 1, 0, 1, 1);
        }
        else if ( i == 2 )
        {
            tex = (txmD3D_TEX *)txmMANAGER__Find(txmManager, "__FULL_8888_02__");
            tex->Create(0, 0, 1, 0, 1, 1);
        }
        else
        {
            tex = hcxe_global_render_targets_tex[i];
            if ( !tex )
            {
                char name[272];
                sprintf_0(name, "__HALO_RT_TEX_%d_", i);
                tex = (txmD3D_TEX *)txmMANAGER__Add(txmManager, name, 360, ETADDR_CLAMP, ETADDR_CLAMP, 1);

                _D3DFORMAT format = global_render_targets[i].format;
                if ( format != D3DFMT_X8R8G8B8 && format != D3DFMT_A8R8G8B8 && !IGNORE_STRONG_ASSERT )
                    STRONG_ASSERT_DUMMY_Crash(0, "0", "D:\\Projects\\code\\HCEX\\sources\\halo_render.cpp", 293, empty_string);

                int created = tex->Create(global_render_targets[i].width, global_render_targets[i].height, 1, 22, 1, 1);
                if ( !IGNORE_STRONG_ASSERT && !created )
                    STRONG_ASSERT_DUMMY_Crash(0, "rc", "D:\\Projects\\code\\HCEX\\sources\\halo_render.cpp", 297, empty_string);
            }
        }

        hcxe_global_render_targets_tex[i] = tex;
        D3DTexture *d3d_texture = txmD3D_TEX__GetD3DTex(tex);
        global_render_targets[i].texture = d3d_texture;
        global_render_targets[i].surface = D3DTexture_GetSurfaceLevel(d3d_texture, 0);
    }

    global_render_targets[9].surface = d3dDriver->pDepthBuffer;
    return D3DResource_AddRef(global_render_targets[9].surface);
}
