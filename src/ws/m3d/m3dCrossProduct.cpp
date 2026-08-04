#include "m3d_boundary.h"

// m3dCrossProduct(const m3dV*, const m3dV*, m3dV*) @ 0x823CD2B8
// Standard right-handed cross product: out = a x b.
void m3dCrossProduct(const m3dV *a, const m3dV *b, m3dV *out)
{
    _m3dCheckValid(a);
    _m3dCheckValid(b);

    out->x = a->y * b->z - a->z * b->y;
    out->y = a->z * b->x - a->x * b->z;
    out->z = a->x * b->y - a->y * b->x;
}
