#include <math.h>
#include "../headers/havok/hkVector4.h"
#include "../headers/havok/hkArray.h"

/* hkpShapeCutterUtil::_calculatePlane — build the plane equation (nx,ny,nz,d) of
   the triangle whose three corner indices are indices[0..2], sourcing positions
   from `vertices`. The normal is the normalized cross product of the two edges
   from corner 0; d = -dot(normal, p0) so that dot(plane.xyz, p) + plane.w == 0 on
   the plane. A degenerate (zero-length) cross yields a zero normal (the original
   selects 0 via vcmpeqfp against |n|^2 == 0).

   DEVIATION: original is VMX128 (vpermwi cross product, vrsqrtefp + one
   Newton-Raphson refinement step for the reciprocal sqrt, vmsum3fp dot products);
   reconstructed as scalar 3-component math using sqrtf. */
void hkpShapeCutterUtil__calculatePlane(const int *indices, const hkArray<hkVector4> *vertices,
                                        hkVector4 *planeOut)
{
    const float *p0 = vertices->m_data[indices[0]].m_quad.___u0.v;
    const float *p1 = vertices->m_data[indices[1]].m_quad.___u0.v;
    const float *p2 = vertices->m_data[indices[2]].m_quad.___u0.v;
    float e1[3], e2[3], n[3], lenSq, scale, d;
    int i;

    for (i = 0; i < 3; ++i)
    {
        e1[i] = p1[i] - p0[i];
        e2[i] = p2[i] - p0[i];
    }

    n[0] = e1[1] * e2[2] - e1[2] * e2[1];
    n[1] = e1[2] * e2[0] - e1[0] * e2[2];
    n[2] = e1[0] * e2[1] - e1[1] * e2[0];

    lenSq = n[0] * n[0] + n[1] * n[1] + n[2] * n[2];
    scale = (lenSq == 0.0f) ? 0.0f : (1.0f / sqrtf(lenSq));

    for (i = 0; i < 3; ++i)
        n[i] *= scale;

    d = -(n[0] * p0[0] + n[1] * p0[1] + n[2] * p0[2]);

    planeOut->m_quad.___u0.v[0] = n[0];
    planeOut->m_quad.___u0.v[1] = n[1];
    planeOut->m_quad.___u0.v[2] = n[2];
    planeOut->m_quad.___u0.v[3] = d;
}
