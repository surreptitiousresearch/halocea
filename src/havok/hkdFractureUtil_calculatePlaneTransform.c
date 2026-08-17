/* ?calculatePlaneTransform@hkdFractureUtil@@SAXABVhkVector4@@0AAVhkMatrix4@@@Z @0x83877378 */
#include "../headers/havok/hkVector4.h"
#include "../headers/havok/hkTransform.h"
#include "../headers/havok/hkMatrix4.h"

extern float sqrtf(float);
extern float fabsf(float);
extern void hkMatrix4_set(hkMatrix4 *out, const hkTransform *t); /* hkMatrix4::set */

/* hkdFractureUtil::calculatePlaneTransform — build an orthonormal frame whose
   x-axis is the plane normal and whose origin is `point`, returned as a 4x4
   matrix. The in-plane tangent is seeded from the plane's least-dominant axis so
   the construction is numerically stable.
   DEVIATION: original is VMX128 (abs via positiveMask, vmsum3fp/vrsqrtefp
   normalize refined against hkQuadRealHalf, vpermwi cross product); reconstructed
   as scalar float. */
void hkdFractureUtil_calculatePlaneTransform(const hkVector4 *point, const hkVector4 *plane, hkMatrix4 *out)
{
    float ax = fabsf(plane->m_quad.___u0.v[0]);
    float ay = fabsf(plane->m_quad.___u0.v[1]);
    float az = fabsf(plane->m_quad.___u0.v[2]);
    int dominant;       /* index of the largest-magnitude plane component */
    int secondary;      /* the paired axis used to seed the tangent */
    float sign = 1.0f;
    float tangent[4];
    hkTransform frame;
    float len2, invLen;
    int i;

    if (ax >= ay)
        dominant = (ax >= az) ? 0 : 2;
    else
        dominant = (ay >= az) ? 1 : 2;

    if (dominant == 0)
        secondary = 1;
    else if (dominant == 1)
    {
        secondary = 0;
        sign = -1.0f;
    }
    else /* dominant == 2 */
    {
        secondary = 1;
        if (plane->m_quad.___u0.v[dominant] > 0.0f)
            sign = -1.0f;
    }

    tangent[0] = tangent[1] = tangent[2] = tangent[3] = 0.0f;
    tangent[secondary] = plane->m_quad.___u0.v[dominant] * sign;
    tangent[dominant]  = -(plane->m_quad.___u0.v[secondary] * sign);

    /* normalize3 of the tangent (w lane stays 0) */
    len2 = tangent[0]*tangent[0] + tangent[1]*tangent[1] + tangent[2]*tangent[2];
    invLen = (len2 == 0.0f) ? 0.0f : (1.0f / sqrtf(len2));
    for (i = 0; i < 4; ++i)
        tangent[i] *= invLen;

    /* col0 = plane normal, translation = point */
    frame.m_rotation.m_col0 = *plane;
    frame.m_translation = *point;
    /* col1 = normalized in-plane tangent */
    for (i = 0; i < 4; ++i)
        frame.m_rotation.m_col1.m_quad.___u0.v[i] = tangent[i];
    /* col2 = plane x tangent */
    {
        float px = plane->m_quad.___u0.v[0], py = plane->m_quad.___u0.v[1], pz = plane->m_quad.___u0.v[2];
        frame.m_rotation.m_col2.m_quad.___u0.v[0] = py*tangent[2] - pz*tangent[1];
        frame.m_rotation.m_col2.m_quad.___u0.v[1] = pz*tangent[0] - px*tangent[2];
        frame.m_rotation.m_col2.m_quad.___u0.v[2] = px*tangent[1] - py*tangent[0];
        frame.m_rotation.m_col2.m_quad.___u0.v[3] = 0.0f;
    }

    hkMatrix4_set(out, &frame);
}
