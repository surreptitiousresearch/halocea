// m3dDist_2(const m3dV*, const m3dV*) @0x8265BC48 — squared Euclidean distance between two points.
// Each component difference is computed at float precision (matching the PPC single-precision
// arithmetic) and validated in debug via _m3dCheckValid; no square root is taken.
#include "m3d_boundary.h"

float m3dDist_2(const m3dV *p1, const m3dV *p2)
{
    _m3dCheckValid(p1);
    _m3dCheckValid(p2);

    float dx = p1->x - p2->x;
    _m3dCheckValid(dx);
    float dy = p1->y - p2->y;
    _m3dCheckValid(dy);
    float dz = p1->z - p2->z;
    _m3dCheckValid(dz);

    return dx * dx + (dz * dz + dy * dy);
}
