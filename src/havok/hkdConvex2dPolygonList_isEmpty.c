#include "../headers/havok/hkdConvex2dPolygonList.h"

/* hkdConvex2dPolygonList::isEmpty — true when the list holds no polygons, i.e.
   the per-polygon vertex-count array is empty. (The DB body computes this as
   countLeadingZeros(m_numVertices.m_size) & 0x20, which is set only when the
   size word is zero.) */
unsigned char hkdConvex2dPolygonList_isEmpty(const hkdConvex2dPolygonList *self)
{
    return (unsigned char)(self->m_numVertices_size == 0);
}
