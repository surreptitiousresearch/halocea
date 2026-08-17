/* ?createBevelPlane@hkGeomConvexHullBuilder@@SAXABVhkVector4@@000AAV?$hkArray@VhkVector4@@@@@Z @0x83FDA7B8 */
#include "../headers/havok/hkGeomConvexHullBuilder_internal.h"
#include <math.h>

/* hkGeomConvexHullBuilder::createBevelPlane
   Appends a bevel plane to planeEquationsOut. The bevel plane is perpendicular to both the
   reference plane normal (planeNormal) and the edge (vertex0 - vertex1); its orientation is
   chosen to point away from vertex2 (so that (c - b).n <= ~0), and it is anchored at vertex0.
   If the constructed normal is degenerate (|n|^2 <= 1e-4) the just-reserved slot is popped
   back off. DEVIATION: cross product / dot / rsqrt-normalize reconstructed as scalar from the
   VMX vpermwi / vmsum3fp / vrsqrtefp+Newton sequences; the cross-product sign convention is
   irrelevant because the vertex2 orientation test forces a deterministic final direction. */
void hkGeomConvexHullBuilder_createBevelPlane(const hkVector4 *planeNormal, const hkVector4 *vertex0,
        const hkVector4 *vertex1, const hkVector4 *vertex2, hkArray<hkVector4> *planeEquationsOut)
{
    const float *n = planeNormal->m_quad.___u0.v;
    const float *a = vertex0->m_quad.___u0.v;
    const float *b = vertex1->m_quad.___u0.v;
    const float *c = vertex2->m_quad.___u0.v;
    float e[3], plane[3], dotC, lenSq;
    hkVector4 *slot;
    int oldSize, newSize, cap, grow;

    /* ---- push one slot ---- */
    oldSize = planeEquationsOut->m_size;
    newSize = oldSize + 1;
    cap = planeEquationsOut->m_capacityAndFlags & 0x3FFFFFFF;
    if (cap < newSize)
    {
        grow = 2 * cap;
        if (newSize >= grow)
            grow = newSize;
        hkArrayUtil__reserve(&planeEquationsOut->m_data, grow, 16);
    }
    planeEquationsOut->m_size = newSize;
    slot = &planeEquationsOut->m_data[oldSize];

    /* ---- plane normal = cross(planeNormal, vertex0 - vertex1) ---- */
    e[0] = a[0] - b[0];
    e[1] = a[1] - b[1];
    e[2] = a[2] - b[2];
    plane[0] = n[1] * e[2] - n[2] * e[1];
    plane[1] = n[2] * e[0] - n[0] * e[2];
    plane[2] = n[0] * e[1] - n[1] * e[0];

    /* orient away from vertex2: (vertex2 - vertex1) . plane <= 0 */
    dotC = plane[0] * (c[0] - b[0]) + plane[1] * (c[1] - b[1]) + plane[2] * (c[2] - b[2]);
    if (dotC > 0.000001f)
    {
        plane[0] = -plane[0];
        plane[1] = -plane[1];
        plane[2] = -plane[2];
    }

    lenSq = plane[0] * plane[0] + plane[1] * plane[1] + plane[2] * plane[2];
    if (lenSq <= 0.000099999997f)
    {
        /* degenerate — pop the slot back off */
        newSize = planeEquationsOut->m_size - 1;
        cap = planeEquationsOut->m_capacityAndFlags & 0x3FFFFFFF;
        if (cap < newSize)
        {
            grow = 2 * cap;
            if (newSize >= grow)
                grow = newSize;
            hkArrayUtil__reserve(&planeEquationsOut->m_data, grow, 16);
        }
        planeEquationsOut->m_size = newSize;
    }
    else
    {
        float inv = 1.0f / sqrtf(lenSq); /* DEVIATION: original vrsqrtefp + 2 Newton steps */
        plane[0] *= inv;
        plane[1] *= inv;
        plane[2] *= inv;
        slot->m_quad.___u0.v[0] = plane[0];
        slot->m_quad.___u0.v[1] = plane[1];
        slot->m_quad.___u0.v[2] = plane[2];
        slot->m_quad.___u0.v[3] = -(plane[0] * a[0] + plane[1] * a[1] + plane[2] * a[2]);
    }
}
