#include "../headers/havok/hkVector4.h"
#include "../headers/havok/hkArray.h"

/* hkArrayUtil::_reserve — SDK-floor boundary (grow/shrink array storage). */
extern void hkArrayUtil__reserve(void *arrayData, int newCapacity, int sizeElem); /* arg0 void* per mangled ?_reserve@hkArrayUtil@@YAXPAXHH@Z */

/* squared 3D distance between two hkVector4 positions (vsubfp + vmsum3fp128). */
static float weld_sqDist3(const hkVector4 *a, const hkVector4 *b)
{
    float dx = a->m_quad.___u0.v[0] - b->m_quad.___u0.v[0];
    float dy = a->m_quad.___u0.v[1] - b->m_quad.___u0.v[1];
    float dz = a->m_quad.___u0.v[2] - b->m_quad.___u0.v[2];
    return dx * dx + dy * dy + dz * dz;
}

/* hkGeomConvexHullBuilder::weldXsortedVertices
   Collapses vertices that lie within weldTolerance (squared distance) of an earlier
   vertex, compacting the survivors to the front of the (already x-sorted) array.
   For each candidate it scans backwards over the recently kept vertices whose x is
   within 0.01 of the candidate; a match welds the candidate away and additionally
   skips any following vertices still within tolerance of the matched point. The
   unique count is written to *numUniqueOut and mirrored into verts->m_size.

   DEVIATION: the binary passes the array pointer in two registers (as hkArray* and
   as (void**)&m_data, identical values); modelled here as one `verts` argument. The
   VMX subtract/dot-product distance is the scalar weld_sqDist3 helper. */
void hkGeomConvexHullBuilder_weldXsortedVertices(float weldTolerance,
                                                 hkArray<hkVector4> *verts,
                                                 int *numUniqueOut)
{
    hkVector4 *base = verts->m_data;
    hkVector4 *rd = base;                 /* read cursor (_R9)  */
    hkVector4 *wr = base;                 /* write cursor (v9)  */
    int remaining = verts->m_size - 1;    /* v8 */
    int count, capacity;

    while (remaining >= 0)
    {
        hkVector4 *back = rd - 1;         /* v10 */
        int welded = 0;

        if (back >= base)
        {
            while (back->m_quad.___u0.v[0] >= rd->m_quad.___u0.v[0] - 0.0099999998f)
            {
                if (weld_sqDist3(back, rd) < weldTolerance)
                {
                    hkVector4 matched = *back;   /* v0 */
                    int inner = remaining - 1;
                    while (inner >= 0)
                    {
                        if (weld_sqDist3(&matched, rd + 1) >= weldTolerance)
                            break;
                        --inner;
                        ++rd;
                        --remaining;
                    }
                    welded = 1;
                    break;
                }
                --back;
                if (back < base)
                    break;
            }
        }

        if (!welded)
        {
            *wr = *rd;
            ++wr;
        }
        --remaining;
        ++rd;
    }

    count = (int)(wr - base);
    *numUniqueOut = count;

    capacity = verts->m_capacityAndFlags & 0x3FFFFFFF;
    if (capacity < count)
    {
        int newCap = 2 * capacity;
        if (count >= newCap)
            newCap = count;
        hkArrayUtil__reserve(&verts->m_data, newCap, 16);
    }
    verts->m_size = count;
}
