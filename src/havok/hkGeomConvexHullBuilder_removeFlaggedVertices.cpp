#include "../headers/havok/hkGeomConvexHullBuilder_internal.h"

/* hkGeomConvexHullBuilder::removeFlaggedVertices
   Stable in-place compaction: keeps only vertices whose w-lane flag is 0.0 (flagged
   vertices carry a non-zero w set by removeCollinearVertices / postFilterVertices) and
   updates the array size. DEVIATION: the original hand-unrolls the scan by 4; the scalar
   loop here is behaviourally identical. The trailing reserve/setSize is the inlined
   hkArray::setSize (never actually grows here since compaction only shrinks). */
void hkGeomConvexHullBuilder_removeFlaggedVertices(hkArray<hkVector4> *verts)
{
    hkVector4 *write = verts->m_data;
    int newSize, cap, read;

    for (read = 0; read < verts->m_size; ++read)
    {
        if (verts->m_data[read].m_quad.___u0.v[3] == 0.0f)
        {
            *write = verts->m_data[read];
            ++write;
        }
    }

    newSize = (int)(write - verts->m_data);
    cap = verts->m_capacityAndFlags & 0x3FFFFFFF;
    if (cap < newSize)
    {
        int grow = 2 * cap;
        if (newSize >= grow)
            grow = newSize;
        hkArrayUtil__reserve(&verts->m_data, grow, 16);
    }
    verts->m_size = newSize;
}
