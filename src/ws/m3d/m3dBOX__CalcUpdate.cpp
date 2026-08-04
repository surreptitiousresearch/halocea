#include "m3dV.h"
#include "m3dBOX.h"

extern void _m3dCheckValid(const m3dV *v); // boundary — vector validity assert, not reversed

// m3dBOX::CalcUpdate(const m3dV*) @ 0x8265C840
// Grow the box to contain point v: bll becomes the per-axis minimum, fur the per-axis
// maximum. The compiler used `fsel` (branchless select on sign of the difference);
// reproduced here as the equivalent ternary min/max.
void m3dBOX::CalcUpdate(const m3dV *v)
{
    _m3dCheckValid(v);

    bll.x = (bll.x - v->x >= 0.0f) ? v->x : bll.x; // min(bll.x, v.x)
    bll.y = (bll.y - v->y >= 0.0f) ? v->y : bll.y; // min(bll.y, v.y)
    bll.z = (bll.z - v->z >= 0.0f) ? v->z : bll.z; // min(bll.z, v.z)
    fur.x = (fur.x - v->x >= 0.0f) ? fur.x : v->x; // max(fur.x, v.x)
    fur.y = (fur.y - v->y >= 0.0f) ? fur.y : v->y; // max(fur.y, v.y)
    fur.z = (fur.z - v->z >= 0.0f) ? fur.z : v->z; // max(fur.z, v.z)
}
