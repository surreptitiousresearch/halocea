#include "../headers/havok/hkGeometry.h"
#include "../headers/havok/hkThreadMemory.h"

/* hkGeometry::~hkGeometry @ 0x8384B9B8
   Frees the triangle and vertex arrays back to the current thread allocator.
   An array whose m_capacityAndFlags has the top (don't-free) bit set is skipped. */
void hkGeometry_destruct(hkGeometry *self)
{
    hkThreadMemory *mem = hkThreadMemory_getCurrent();
    int triCap = self->m_triangles.m_capacityAndFlags;
    int vtxCap;

    if (triCap >= 0)
        hkThreadMemory_deallocateChunk(mem, self->m_triangles.m_data,
                                       12 * (triCap & 0x3FFFFFFF), HK_MEMORY_CLASS_ARRAY);

    vtxCap = self->m_vertices.m_capacityAndFlags;
    if (vtxCap >= 0)
        hkThreadMemory_deallocateChunk(mem, self->m_vertices.m_data,
                                       16 * vtxCap, HK_MEMORY_CLASS_ARRAY);
}
