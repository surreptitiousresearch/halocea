#include "../../headers/ws/rend/rendDRIVER.h"

// rendDRIVER::DrawPoly @ 0x827C91E8
// Draw a single convex polygon as an unindexed triangle fan. First clears the current
// transform (vtable slot 0x3C -> SetTransform(nullptr), disasm 0x827C9204-0x827C9210),
// then submits via DynGeomDraw with no index buffer.
//
// Deviation/note: the disasm (0x827C9214-0x827C921C) loads r9 = 0x594C4F50 ('POLY' FourCC)
// and pairs it with the incoming `fvfHigh` (r8) to form the 64-bit fvf/tag word passed to
// DynGeomDraw. The decompiler renders this as HIDWORD(fvf) = 0x594C4F50 with an
// uninitialized callerID; transcribed here to match the observed register pairing.
void rendDRIVER::DrawPoly(void *vertList, int nVert, int fvfHigh, unsigned __int64 fvf)
{
    SetTransform(nullptr);

    // fvf word: high dword = incoming fvfHigh, low dword = 'POLY' debug tag (0x594C4F50).
    unsigned __int64 fvfTagged = ((unsigned __int64)(unsigned int)fvfHigh << 32) | 0x594C4F50u;
    unsigned int callerID = 0; // r10 never set at the call site — passed through as-is

    DynGeomDraw(REND_PRIMTYPE_TRIFAN, vertList, nVert, nullptr, 0, fvfTagged, callerID);
}
