/* ?Flush@UI_STAT_RENDER@ui_new@@QAAXXZ @0x82CEDC68 */
#include "../../headers/ws/ui_new/UI_STAT_RENDER.h"
#include "../../headers/ws/ui_new/ui_stat_render_boundary.h"
#include "../../headers/ws/txm/txmMANAGER.h"

extern rendDRIVER *rendDrv;

// 0x82CEDC7C — draw the batched quads in one indexed-primitive call, then reset the buffers.
// NOTE: rendDRIVER::Configure's state-descriptor argument (`stateDesc` below) is an opaque blob
// belonging to the un-reversed rendDRIVER::Configure boundary (see rendDRIVER.h) — its true field
// layout/semantics are UNRESOLVED. The bytes below reproduce, at their literal relative stack
// offsets from the decompiled pseudocode, the exact constant values this call site writes (two
// 1-bytes, four 1.0f floats, then 1/0/0/0/0/1/1, then the bound texture pointer) without asserting
// what each field means.
void ui_new::UI_STAT_RENDER::Flush()
{
    if (!pTex.ptr)
        return;
    if (!vBuf.nElem)
        return;
    if (!iBuf.nElem)
        return;

    struct /* UNRESOLVED: rendDRIVER::Configure state-descriptor layout */
    {
        unsigned char  b0, b1;              // sp+50/51 — both 1
        float          f0, f1, f2, f3;      // sp+54..60 — all 1.0f
        unsigned char  c0, c1, c2, c3, c4, c5, c6; // sp+64..6A — 1,0,0,0,0,1,1
        txmTEXTURE_PTR tex;                 // sp+6C — bound texture
    } stateDesc = { 1, 1, 1.0f, 1.0f, 1.0f, 1.0f, 1, 0, 0, 0, 0, 1, 1, {nullptr} };
    stateDesc.tex = pTex;

    // empty_string is the empty page-name string at 0x8200155A; SetCurDip's nameDip is const char*.
    vidDriver->dipStat.SetCurDip("UI", (const char *)empty_string, 0);
    rendDRIVER_Configure(rendDrv, 0, &stateDesc, 0x2020495540000000ULL, COORDSPACE_SCREEN, RENDERBLOCK_COLOR, 0, 0);
    rendDRIVER_DrawIndexedPrimitive(rendDrv, vBuf.pData, vBuf.nElem, iBuf.pData, iBuf.nElem / 3,
                                     0x2020495540000000ULL, 0);

    Reset();

    if (stateDesc.tex.ptr && txmManager)
        stateDesc.tex.ptr->Release();
}
