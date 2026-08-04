#include "m3dMATR.h"

// m3dMATR::Scale @ 0x82542AC0
// Apply a per-axis scale to the matrix. `oper` selects the mode:
//   0 — reset scale first (RemoveScale), then scale each column (incl. translation) in world axes.
//   1 — scale each basis row by the matching factor (local-axis scale of the rotation only).
//   2 — scale each column (incl. translation) in world axes, without resetting first.
// any other value is a no-op.
//
// ABI: the mangle is ?Scale@m3dMATR@@QAAXMMMH@Z — three float args (f1..f3) then int `oper`
// in r7. The decompiler mis-shifted the trailing GPR and fabricated phantom params (a6..a8);
// verified against the prologue (cmpwi cr6,r7,{0,1,2}) that the switch is on `oper`.
void m3dMATR::Scale(float sx, float sy, float sz, int oper)
{
    if (oper == 1)
    {
        // Scale each basis row (v[0] by sx, v[1] by sy, v[2] by sz); translation untouched.
        elements[0] *= sx;  elements[1] *= sx;  elements[2] *= sx;
        elements[4] *= sy;  elements[5] *= sy;  elements[6] *= sy;
        elements[8] *= sz;  elements[9] *= sz;  elements[10] *= sz;
        return;
    }

    if (oper != 0 && oper != 2)
        return;

    if (oper == 0)
        RemoveScale();

    // Scale each column: x-components by sx, y-components by sy, z-components by sz,
    // across all four rows including the translation row.
    elements[0] *= sx;   elements[1] *= sy;   elements[2] *= sz;
    elements[4] *= sx;   elements[5] *= sy;   elements[6] *= sz;
    elements[8] *= sx;   elements[9] *= sy;   elements[10] *= sz;
    elements[12] *= sx;  elements[13] *= sy;  elements[14] *= sz;
}
