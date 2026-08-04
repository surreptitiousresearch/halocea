#include "../headers/havok/hkVector4.h"
#include "../headers/havok/hkTransform.h"

/* Boundary callees (2-level descent). */
extern void hkdShapePlaneConnectivityCalculator_calculateBasisX(const hkVector4 *normal,
                                                                const hkVector4 *pos,
                                                                hkTransform *trans); /* ::calculateBasisX */
extern void hkTransform_setInverse(hkTransform *out, const hkTransform *t);          /* hkTransform::setInverse */

/* hkdShapePlaneConnectivityCalculator::calculateWorldToPlaneTransform — build the
   rigid transform that maps world space into the local frame of a plane. A point
   on the plane is taken as normal * (-planeDistance) (the plane is packed as
   (nx,ny,nz,d)); calculateBasisX builds a plane-aligned basis at that point, and
   the world->plane transform is its inverse.

   DEVIATION: the point computation is a VMX128 splat-multiply (each lane of the
   plane vector scaled by -w); reproduced here as a per-lane scalar multiply. */
void hkdShapePlaneConnectivityCalculator_calculateWorldToPlaneTransform(const hkVector4 *plane,
                                                                        hkTransform *worldToPlane)
{
    float negDistance = -plane->m_quad.___u0.vector4_f32[3];
    hkVector4 pointOnPlane;
    hkTransform planeToWorld;
    int lane;

    for (lane = 0; lane < 4; ++lane)
        pointOnPlane.m_quad.___u0.vector4_f32[lane] =
            plane->m_quad.___u0.vector4_f32[lane] * negDistance;

    hkdShapePlaneConnectivityCalculator_calculateBasisX(plane, &pointOnPlane, &planeToWorld);
    hkTransform_setInverse(worldToPlane, &planeToWorld);
}
