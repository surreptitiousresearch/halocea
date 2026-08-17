/* ?sort@PlaneAndPoints@hkGeomConvexHullBuilder@@QAAXXZ @0x83FD9F78 */
#include "../headers/havok/hkGeomConvexHullBuilder_internal.h"

/* hkGeomConvexHullBuilder::PlaneAndPoints::sort
   Sorts the three half-edge pointers (m_v0, m_v1, m_v2) of the candidate face plane
   ascending by their referenced vertex index. A hard-coded 3-element sorting network:
   compare-swap (v0,v1), then (v1,v2), then (v0,v1). Vertex indices compared unsigned. */
void hkGeomConvexHullBuilder_PlaneAndPoints_sort(hkGeomConvexHullBuilder_PlaneAndPoints *self)
{
    hkGeomEdge *tmp;

    if ((unsigned int)self->m_v0->m_vertex > (unsigned int)self->m_v1->m_vertex)
    {
        tmp = self->m_v0;
        self->m_v0 = self->m_v1;
        self->m_v1 = tmp;
    }
    if ((unsigned int)self->m_v1->m_vertex > (unsigned int)self->m_v2->m_vertex)
    {
        tmp = self->m_v1;
        self->m_v1 = self->m_v2;
        self->m_v2 = tmp;
    }
    if ((unsigned int)self->m_v0->m_vertex > (unsigned int)self->m_v1->m_vertex)
    {
        tmp = self->m_v0;
        self->m_v0 = self->m_v1;
        self->m_v1 = tmp;
    }
}
