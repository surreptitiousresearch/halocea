// cdtINFO::cdtINFO @0x82C16018 — zero the whole record (bitfields, the three opaque id fields,
// vClosest, distMin, state); the raw decompile's 9-word zero loop covers the entire 36-byte object.
#include "../../headers/ws/cdt/cdtINFO.h"

cdtINFO::cdtINFO()
{
    reserved_bits = 0;
    SetVClosestCalled = 0;
    SetDistMinCalled = 0;
    _pad1[0] = _pad1[1] = _pad1[2] = 0;
    ___u2.pData = 0;   /* DB anonymous union arm */
    ___u3.id = 0;
    ___u4.id2 = 0;
    vClosest.x = vClosest.y = vClosest.z = 0.0f;
    distMin = 0.0f;
    state.state = 0;
}
