#include <math.h>
#include "m3d_boundary.h"

// m3dVRand(const m3dV&, unsigned long) @ 0x82562A80
//
// CAVEAT: NOT a stateful LCG (unlike m3dRND_GEN::RndInt) -- this is a deterministic
// position-based hash: it scales the vector by 10, floors/truncates each axis to an
// integer, divides the resulting integer triple by 1000 to fold it back into [0,1)-ish
// per-axis "noise cells", takes the fractional part of each axis, multiplies each by a
// large float "mixing" constant (values reproduced verbatim from the disasm literals:
// 462480480.0 for y, 768004672.0 for z, 605608960.0 for x) truncating toward zero to a
// 32-bit hash lane, then bit-mixes the three lanes with shifts/XORs and multiplies the
// result by `stride`. Reconstructed instruction-for-instruction from the disasm (the
// decompiler's C output for this function has OVERLAPPED double/int64 packing artifacts
// and could not be used directly). Verify against disasm if this is ever suspected wrong.
unsigned int m3dVRand(const m3dV &vv, unsigned int stride)
{
    _m3dCheckValid(10.0f);
    _m3dCheckValid(&vv);

    float scaledZ = vv.z * 10.0f;
    float scaledY = vv.y * 10.0f;
    float scaledX = vv.x * 10.0f;

    int cellZ = (int)floorf(scaledZ);
    int cellY = (int)floorf(scaledY);
    int cellX = (int)floorf(scaledX);

    m3dV cell;
    cell.x = (float)cellX;
    cell.y = (float)cellY;
    cell.z = (float)cellZ;
    cell /= 1000.0f;

    float dummyIntPart;
    float fracX = modff(cell.x, &dummyIntPart);
    float fracY = modff(cell.y, &dummyIntPart);
    float fracZ = modff(cell.z, &dummyIntPart);

    unsigned int hashY = (unsigned int)(int)(fracY * 462480480.0f);
    unsigned int hashZ = (unsigned int)(int)(fracZ * 768004672.0f);
    unsigned int hashX = (unsigned int)(int)(fracX * 605608960.0f);

    unsigned int mixed = ((hashY >> 4) ^ hashZ) >> 3;
    unsigned int mixedHigh = (hashZ << 8) ^ hashY;
    unsigned int hash = (hashX ^ mixed) ^ (mixedHigh << 8);

    return hash * stride;
}
