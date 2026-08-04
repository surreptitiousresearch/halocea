#include "m3d_boundary.h"

// m3dNormalComponent(const m3dV*, const m3dV*, m3dV*) @ 0x825419F0
// DEVIATION from the name's apparent implication: this returns the component of `v` that
// lies ALONG `axis` (the projection of v onto axis, i.e. (v . axis) * axis), not the
// orthogonal residual -- confirmed by the disasm, which stores axis.{x,y,z} * dot into
// out.{x,y,z} with no subtraction of v itself.
void m3dNormalComponent(const m3dV *v, const m3dV *axis, m3dV *out)
{
    float dot = v->x * axis->x + v->z * axis->z + v->y * axis->y;

    _m3dCheckValid(axis);
    _m3dCheckValid(dot);

    out->x = axis->x * dot;
    out->y = axis->y * dot;
    out->z = axis->z * dot;
}
