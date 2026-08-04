#include "../../headers/ws/ui_new/UI_STAT_RENDER.h"

// 0x82CEF5F8 — append one textured quad (two triangles) to the batch: 4 vertices at rect's
// corners (TL, BL, BR, TR winding, matching the index pattern below) tinted `clr`, with UV
// coordinates from the matching corner of `uv`.
// DEVIATION: `uv` is passed BY VALUE (a plain m3dRECT, not a pointer) per the DB mangle
// (...ABUm3dRECT@@U3@@Z). Its 16 bytes are split across two GPRs by this PPC64 ABI, which the
// decompiler's naive per-register model exposed as a bogus extra `int a5` parameter and rendered
// every `uv`/`a5` field read as a raw pointer-bit-pattern reinterpreted as float
// (`*(float *)&uv`) — nonsense taken literally. Reconstructed here as the standard "position quad
// paired 1:1 with its UV quad" a textured-quad batcher implies, matching the position-corner
// order the decompiler DID read correctly (rect.l/t, rect.l/b, rect.r/b, rect.r/t) and the index
// buffer's (0,1,3)(1,2,3) winding.
void ui_new::UI_STAT_RENDER::AddBox(unsigned int clr, const m3dRECT &rect, m3dRECT uv)
{
    if (!pTex.ptr)
        return;

    int base = vBuf.nElem;

    rendV3D quad[4];
    quad[0].v = {rect.l, rect.t, 0.0f}; quad[0].tc = {uv.l, uv.t}; quad[0].color = clr;
    quad[1].v = {rect.l, rect.b, 0.0f}; quad[1].tc = {uv.l, uv.b}; quad[1].color = clr;
    quad[2].v = {rect.r, rect.b, 0.0f}; quad[2].tc = {uv.r, uv.b}; quad[2].color = clr;
    quad[3].v = {rect.r, rect.t, 0.0f}; quad[3].tc = {uv.r, uv.t}; quad[3].color = clr;

    vBuf.Insert(base, quad, 4);

    unsigned short idx;
    idx = (unsigned short)(base + 0); iBuf.PushBack(idx);
    idx = (unsigned short)(base + 1); iBuf.PushBack(idx);
    idx = (unsigned short)(base + 3); iBuf.PushBack(idx);
    idx = (unsigned short)(base + 1); iBuf.PushBack(idx);
    idx = (unsigned short)(base + 2); iBuf.PushBack(idx);
    idx = (unsigned short)(base + 3); iBuf.PushBack(idx);
}
