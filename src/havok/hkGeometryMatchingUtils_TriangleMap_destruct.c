/* FUNCTION_INDEX entry: hkGeometryMatchingUtils_TriangleMap_destruct @0x83845950 (??1TriangleMap@hkGeometryMatchingUtils@@QAA@XZ) */
#include "../headers/havok/hkGeometryMatchingUtils_TriangleMap.h"
#include "../headers/havok/hkThreadMemory.h"

/* hkGeometryMatchingUtils::TriangleMap::~TriangleMap — free both inplace arrays.
   Each is released only when it owns heap storage (m_capacityAndFlags >= 0, i.e.
   the DONT_DEALLOCATE flag 0x80000000 is clear, so the buffer is not the inline
   m_storage). Byte sizes: Hit is 12 bytes/elem, the start-index array is 4. */
void hkGeometryMatchingUtils_TriangleMap_destruct(hkGeometryMatchingUtils_TriangleMap *self)
{
    hkThreadMemory *mem = hkThreadMemory_getCurrent();
    int cap;

    cap = self->m_foundReferenceTriangle.m_capacityAndFlags;
    if (cap >= 0)
        hkThreadMemory_deallocateChunk(mem, self->m_foundReferenceTriangle.m_data,
                                       12 * (cap & 0x3FFFFFFF), HK_MEMORY_CLASS_ARRAY);

    cap = self->m_startIndexPerGeometry.m_capacityAndFlags;
    if (cap >= 0)
        hkThreadMemory_deallocateChunk(mem, self->m_startIndexPerGeometry.m_data,
                                       4 * cap, HK_MEMORY_CLASS_ARRAY);
}
