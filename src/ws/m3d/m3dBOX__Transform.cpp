#include "m3dBOX.h"
#include "m3dMATR.h"
#include "m3dOBB.h"
#include "m3d_boundary.h"

// m3dBOX::Transform @ 0x8265E690  (base overload — lower address)
// Transform this axis-aligned box by an LCS->WCS matrix into an oriented box: origin =
// transformed min corner, axes = normalised matrix basis vectors, and the extents = box
// dimensions scaled by the (pre-normalisation) axis lengths.
void m3dBOX::Transform(m3dMATR *pLCS2WCS, m3dOBB *obb)
{
    pLCS2WCS->TransformPoint(&bll, &obb->org);

    obb->vx = pLCS2WCS->v[0];   // slices m4dV -> m3dV (drops w)
    obb->vy = pLCS2WCS->v[1];
    obb->vz = pLCS2WCS->v[2];

    float lengthX = m3dLengthVector(&obb->vx);
    float lengthY = m3dLengthVector(&obb->vy);
    float lengthZ = m3dLengthVector(&obb->vz);

    _m3dCheckValid(&obb->vx);
    float invLengthX = 1.0f / lengthX;
    _m3dCheckValid(invLengthX);
    obb->vx.x *= invLengthX;
    obb->vx.y *= invLengthX;
    obb->vx.z *= invLengthX;

    _m3dCheckValid(&obb->vy);
    float invLengthY = 1.0f / lengthY;
    _m3dCheckValid(invLengthY);
    obb->vy.x *= invLengthY;
    obb->vy.y *= invLengthY;
    obb->vy.z *= invLengthY;

    _m3dCheckValid(&obb->vz);
    float invLengthZ = 1.0f / lengthZ;
    _m3dCheckValid(invLengthZ);
    obb->vz.x *= invLengthZ;
    obb->vz.y *= invLengthZ;
    obb->vz.z *= invLengthZ;

    obb->sx = (fur.x - bll.x) * lengthX;
    obb->sy = (fur.y - bll.y) * lengthY;
    obb->sz = (fur.z - bll.z) * lengthZ;
}
