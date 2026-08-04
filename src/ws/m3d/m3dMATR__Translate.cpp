#include "m3dMATR.h"

// m3dMATR::Translate(float,float,float,int) @ 0x82541F10  (base overload — lower address)
// Apply a translation to the matrix origin (v[3]) in one of three modes selected by `oper`:
//   0: set the origin absolutely;
//   1: translate along the matrix's own (local) axes;
//   2: add the offset directly in world space;
//  >=3: no-op.
// DEVIATION: the float-slot-skip ABI led the decompiler to fabricate phantom int args
// (a6/a7/a8) and switch on a8; the disassembly shows the discriminant is r7 == `oper`.
void m3dMATR::Translate(float x, float y, float z, int oper)
{
    switch (oper)
    {
    case 0:
        v[3].x = x;
        v[3].y = y;
        v[3].z = z;
        break;
    case 1:
        v[3].x += v[0].x * x + v[1].x * y + v[2].x * z;
        v[3].y += v[0].y * x + v[1].y * y + v[2].y * z;
        v[3].z += v[0].z * x + v[1].z * y + v[2].z * z;
        break;
    case 2:
        v[3].x += x;
        v[3].y += y;
        v[3].z += z;
        break;
    default:
        break;
    }
}
