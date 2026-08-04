#include "m3dV.h"
#include "m3dMATR.h"
#include "m3d_boundary.h"

// m3dMATR::TransformPoint @ 0x82541C48
// Affine transform of a point: result = p.x*axisX + p.y*axisY + p.z*axisZ + origin,
// where the basis axes are rows v[0..2] and the origin is v[3].
// DEVIATION: the decompiler emitted the raw VMX (lvlx128/vmaddfp/stvewx128) sequence;
// it is the standard row-major affine point transform, reconstructed here in scalar form.
void m3dMATR::TransformPoint(const m3dV *p, m3dV *pResult) const
{
    _m3dCheckValid(p);

    pResult->x = p->x * v[0].x + p->y * v[1].x + p->z * v[2].x + v[3].x;
    pResult->y = p->x * v[0].y + p->y * v[1].y + p->z * v[2].y + v[3].y;
    pResult->z = p->x * v[0].z + p->y * v[1].z + p->z * v[2].z + v[3].z;
}
