// ws-engine ai08 (a8_debug.cpp): aiDEBUG::RenderDebugInfoMesh @0x83194690 (static).
// Submit an indexed debug triangle mesh (vertex/index dsVECTORs) to the render driver at the given
// alpha, with an optional depth test. Builds the render-pass descriptor on the stack, configures
// the driver, and draws. 4 params (the decompiler's spurious 5th arg was disasm-verified away).
#include "../../headers/ws/ai/aiDEBUG.h"
#include "../../headers/ws/ai/aiDEBUG_boundaries.h"     // aiDBGSHAPE_VERT / rendDrv
#include "../../headers/ws/ai/aiDBG_MESH_PASS_DESC.h"
#include "../../headers/ws/rend/rendDRIVER.h"
#include "../../headers/ws/txm/txmTEXTURE.h"
#include "../../headers/ws/txm/txmMANAGER.h"  // txmMANAGER type + extern txmManager singleton

void aiDEBUG::RenderDebugInfoMesh(const dsVECTOR<aiDBGSHAPE_VERT, 8> &vBuf,
                                  dsVECTOR<unsigned short, 8> &iBuf, float alpha, bool zTest)
{
    aiDBG_MESH_PASS_DESC cfg;
    cfg.enable      = 1;
    cfg.flag01      = 0;
    cfg.colorScaleR = 1.0f;
    cfg.colorScaleG = 1.0f;
    cfg.colorScaleB = 1.0f;
    cfg.alpha       = alpha;
    cfg.flag14      = 1;
    cfg.flag15      = 0;
    cfg.flag16      = 0;
    cfg.zTest       = zTest ? 1 : 0;
    cfg.flag18      = 1;
    cfg.flag19      = 0;
    cfg.flag1A      = 0;
    cfg.texture     = nullptr;

    // space=COORDSPACE_LOCAL(0), rend_block=RENDERBLOCK_COLOR(0), no dyn/SM lights (disasm-verified
    // all-zero tail args). a4 = 0x40000001.
    rendDrv->Configure(0, &cfg, 0x40000001, 0, /*COORDSPACE_LOCAL*/ 0,
                       /*RENDERBLOCK_COLOR*/ 0, 0, 0);

    // fvf = 0x40000001445F4941, callerID = 3 (disasm-verified). NOTE: the DB mangling for
    // DrawIndexedPrimitive is 6 params; this header carries a spurious `a6` slot (passed 0).
    rendDrv->DrawIndexedPrimitive(vBuf.pData, vBuf.nElem, iBuf.pData, iBuf.nElem / 3,
                                  /*a6*/ 0, 0x40000001445F4941ULL, 3);

    if (cfg.texture && txmManager)
        cfg.texture->Release();
}
