// _m3dLineCombine @0x823CD1A0 — destructive line-combine: out = base + dir * t.
// (?_m3dLineCombine@@YAXPIBUm3dV@@0MPIAU1@@Z) Validates the two input vectors and the scalar in
// debug builds, then writes the combined vector to `out` (which may alias `base`).
#include "../../headers/ws/m3d/m3d_boundary.h"

void _m3dLineCombine(const m3dV *base, const m3dV *dir, float t, m3dV *out)
{
    _m3dCheckValid(base);
    _m3dCheckValid(dir);
    _m3dCheckValid(t);

    // Evaluate y/z before the aliasing store to x (out may == base).
    float y = dir->y * t + base->y;
    float z = dir->z * t + base->z;
    out->x = dir->x * t + base->x;
    out->y = y;
    out->z = z;
}
