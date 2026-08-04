#pragma once
#include "m3dSPL.h"
// ws-engine m3d: a piecewise-linear 1-D spline. DB-verified layout (types_members m3dSPL_LINEAR1D):
// a single m3dSPL base at offset 0 (size 20, no added data). Evaluation (GetValue) is dispatched
// through the m3dSPL vtable slot; nKp / GetKpArg / GetKpValue come from the m3dSPL base. Previously
// kept as an opaque 20-byte blob; now expanded to the real base type since the spline is a frontier.

struct m3dSPL_LINEAR1D : m3dSPL {
    // ?GetValue@m3dSPL_LINEAR1D@@UBAXMPAX0@Z — virtual const: evaluate the spline at parameter `t`,
    // writing the interpolated value into `outValue` (and an auxiliary/second output into `outAux`).
    // The real call sites dispatch via __vftable->GetValue; kept as a member for source clarity.
    // boundary.
    void GetValue(float t, void *outValue, void *outAux) const;
};
