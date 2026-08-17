/* ??0hkGeomHull@@QAA@XZ @0x83FE0C28 */
#include "../headers/havok/hkGeomHull.h"

/* hkGeomHull::hkGeomHull — initialise an empty hull whose edge list starts in the
   128-entry inline storage (capacity 128 with the DONT_DEALLOCATE sign bit set, so
   it only spills to the heap once it grows past 128 edges). The constructor pre-
   clears the m_info word of every inline edge slot.

   DEVIATION: the original walks the storage with a 16-bit pointer, writing a zero
   to each edge's m_info (offset 6) on an 8-byte stride; reproduced as a scalar
   loop over the 128 inline edges. The trailing redundant `m_size = 0` in the
   decompile is dropped (already set). */
void hkGeomHull_construct(hkGeomHull *self)
{
    int i;

    self->m_vertexBase = nullptr;
    self->m_edges.m_data = self->m_edges.m_storage;
    self->m_edges.m_size = 0;
    self->m_edges.m_capacityAndFlags = (int)0x80000080; /* 128 | DONT_DEALLOCATE */

    for (i = 0; i < 128; ++i)
        self->m_edges.m_storage[i].m_info = 0;
}
