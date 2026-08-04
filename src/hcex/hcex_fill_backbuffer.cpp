/* hcex_fill_backbuffer @0x823F0D30 — HCEX bridge: run the final gamma pass into the back buffer. It builds a
 * PASS_DESC_FILL from the video driver's gamma/contrast/brightness settings, binds the HDR colour render
 * target and the gamma-lookup texture, configures the ws render driver for a screen-space colour pass, and
 * draws a full-screen textured quad. All drivers/managers are ws-engine boundaries.
 *
 * CAVEAT: the two 64-bit driver descriptor words are recovered from the disassembly (r6 == 0x1040000001 for
 * both rendDRIVER::Configure and rendDRIVER::DrawPoly); the decompiler split them into separate int/enum
 * arguments. They encode the coord-space/render-block/vertex-format the ws driver expects for a screen quad.
 * The four quad vertices leave their packed-colour field uninitialised — the function never writes it (the
 * gamma pass ignores diffuse colour), matching the disassembly. */

#include "../headers/hcex/hcex_fill_backbuffer_boundary.h"
#include "../headers/dynamic_screen_vertex.h"

#define RENDER_STATE_FILL_ACTIVE   0x10000000u   /* d3dDRIVER::state bit toggled around the start-render call */
#define SCREEN_QUAD_COORD_DESC      0x0000001040000001ULL  /* packed coord-space / vertex-format descriptor */

extern "C" void hcex_fill_backbuffer(void)
{
    d3dDriver->base.state.state |= RENDER_STATE_FILL_ACTIVE;
    d3dDRIVER_SetStartRenderingState(d3dDriver);
    d3dDriver->base.state.state &= ~RENDER_STATE_FILL_ACTIVE;

    txmD3D_TEX *color_target = hcxe_global_render_targets_tex[1];
    vidMODE    *mode = vidDriver->pMode;
    int         width_px  = mode->sx;
    int         height_px = mode->sy;

    PASS_DESC_FILL desc;
    desc.colorConst.a = 0.0f;
    desc.colorConst.r = 0.0f;
    desc.colorConst.g = 0.0f;
    desc.writeDepth = 0;
    desc.colorConst.b = 0.0f;
    desc.depthDownsampleNormals = 0;
    memset(&desc.texColor, 0, 32);   /* clear texColor..texBicubicLookup */
    desc.pass = PASS_DESC_FILL::PASS_GAMMA;
    desc.colorConst.r = vidDriver->gammaCfg.gamma;
    desc.colorConst.g = vidDriver->gammaCfg.contrast;
    desc.colorConst.b = vidDriver->gammaCfg.brightness;
    desc.colorConst.a = 0.0f;

    if ( color_target )
        osLockedIncrement(&color_target->refCount);
    desc.texColor.ptr = (txmTEXTURE *)color_target;
    desc.writeDepth = 0;
    desc.writeColor = 1;

    txmTEXTURE *gamma_lookup = txmManager->gammaLookup.ptr;
    if ( gamma_lookup )
        osLockedIncrement(&gamma_lookup->refCount);
    desc.texGamma.ptr = gamma_lookup;

    rendDRIVER_Configure(rendDrv, 6, &desc, SCREEN_QUAD_COORD_DESC, 2, 0, 0);

    float width  = (float)width_px;
    float height = (float)height_px;

    /* Full-screen quad: corners (0,0)->(w,0)->(w,h)->(0,h) with UVs (0,0)->(1,0)->(1,1)->(0,1). */
    dynamic_screen_vertex quad[4];
    quad[0].position.x = 0.0f;  quad[0].position.y = 0.0f;    quad[0].position.z = 0.0f;
    quad[0].texcoord.x = 0.0f;  quad[0].texcoord.y = 0.0f;
    quad[1].position.x = width; quad[1].position.y = 0.0f;    quad[1].position.z = 0.0f;
    quad[1].texcoord.x = 1.0f;  quad[1].texcoord.y = 0.0f;
    quad[2].position.x = width; quad[2].position.y = height;  quad[2].position.z = 0.0f;
    quad[2].texcoord.x = 1.0f;  quad[2].texcoord.y = 1.0f;
    quad[3].position.x = 0.0f;  quad[3].position.y = height;  quad[3].position.z = 0.0f;
    quad[3].texcoord.x = 0.0f;  quad[3].texcoord.y = 1.0f;

    rendDRIVER_DrawPoly(rendDrv, quad, 4, SCREEN_QUAD_COORD_DESC);
    PASS_DESC_FILL_dtor(&desc);
}
