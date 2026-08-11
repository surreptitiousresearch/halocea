/* FUNCTION_INDEX entry: hkdConvex2dPolygonList_dtor @0x83877098 (??1hkdConvex2dPolygonList@@QAA@XZ) */
#include "../headers/havok/hkdConvex2dPolygonList.h"
#include "../headers/havok/hkThreadMemory.h"

/* hkdConvex2dPolygonList::~hkdConvex2dPolygonList — release the two inline arrays'
   heap storage. Each array frees its buffer only when heap-owned (capacity sign
   bit clear); the vertex-count array holds 1-byte elements and the vertex array
   holds 8-byte hkdVector2 elements, so their freed sizes differ accordingly. */
void hkdConvex2dPolygonList_dtor(hkdConvex2dPolygonList *self)
{
    hkThreadMemory *tm = hkThreadMemory_getCurrent();

    if (self->m_numVertices_capacityAndFlags >= 0)
        hkThreadMemory_deallocateChunk(tm, self->m_numVertices_data,
                                       self->m_numVertices_capacityAndFlags & 0x3FFFFFFF,
                                       HK_MEMORY_CLASS_ARRAY);

    if (self->m_vertices_capacityAndFlags >= 0)
        hkThreadMemory_deallocateChunk(tm, self->m_vertices_data,
                                       8 * self->m_vertices_capacityAndFlags,
                                       HK_MEMORY_CLASS_ARRAY);
}
