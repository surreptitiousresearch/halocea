#include "../headers/havok/hkVector4.h"
#include "../headers/havok/hkTransform.h"
#include "../headers/havok/hkRotation.h"

/* boundary callees */
extern void hkTransform_setInverse(hkTransform *out, const hkTransform *in);            /* hkTransform::setInverse */
/* 0x83F94A90 calls ?_setRotatedDir@hkVector4@@QAAXABVhkRotation@@ABV1@@Z (0x83601070) — the
   hkRotation overload, whose DB prototype is
   void hkVector4___setRotatedDir(hkVector4 *this, const hkRotation *t, const hkVector4 *v). */
extern void hkVector4_setRotatedDir(hkVector4 *out, const hkRotation *rot, const hkVector4 *dir); /* hkVector4::setRotatedDir */

/* hkpShapeCutterUtil::inverseTransformPlane
   Transform a plane (nx,ny,nz,d) by the inverse of `transIn`, writing the result
   to `planeOut`. The rotated normal is R^T * n; the distance term is corrected by
   the projection of the inverse translation.

   DEVIATION: the original is entirely VMX128 (lvx128/vmsum3fp128/vrlimi128 …);
   reconstructed here as scalar float math over the 4 lanes. */
void hkpShapeCutterUtil_inverseTransformPlane(const hkVector4 *plane, const hkTransform *transIn, hkVector4 *planeOut)
{
    hkTransform inv;
    hkRotation invRot;
    float negInvTrans[3];
    float dotCol0, dotCol1, dotCol2;
    float distanceCorrection;
    int i;

    hkTransform_setInverse(&inv, transIn);
    invRot = inv.m_rotation;

    /* g = -(inverse translation) */
    for (i = 0; i < 3; ++i)
        negInvTrans[i] = -inv.m_translation.m_quad.___u0.v[i];

    /* per-column dot of the inverse rotation with g */
    dotCol0 = invRot.m_col0.m_quad.___u0.v[0] * negInvTrans[0]
            + invRot.m_col0.m_quad.___u0.v[1] * negInvTrans[1]
            + invRot.m_col0.m_quad.___u0.v[2] * negInvTrans[2];
    dotCol1 = invRot.m_col1.m_quad.___u0.v[0] * negInvTrans[0]
            + invRot.m_col1.m_quad.___u0.v[1] * negInvTrans[1]
            + invRot.m_col1.m_quad.___u0.v[2] * negInvTrans[2];
    dotCol2 = invRot.m_col2.m_quad.___u0.v[0] * negInvTrans[0]
            + invRot.m_col2.m_quad.___u0.v[1] * negInvTrans[1]
            + invRot.m_col2.m_quad.___u0.v[2] * negInvTrans[2];

    distanceCorrection = plane->m_quad.___u0.v[0] * dotCol0
                       + plane->m_quad.___u0.v[1] * dotCol1
                       + plane->m_quad.___u0.v[2] * dotCol2;

    hkVector4_setRotatedDir(planeOut, &invRot, plane);
    planeOut->m_quad.___u0.v[3] = plane->m_quad.___u0.v[3] + distanceCorrection;
}
