// FUNCTION_INDEX entry: hkdAssetProcessingUtil_BaseObjectSource_destruct @0x838445B8 (??1hkdAssetProcessingUtil_BaseObjectSource@@QAA@XZ)
#include "../headers/havok/hkdAssetProcessingUtil_BaseObjectSource.h"
#include "../headers/havok/hkThreadMemory.h"

/* hkdAssetProcessingUtil::BaseObjectSource::~BaseObjectSource — free the two
   per-section count arrays (int, 4 bytes/elem) when they own heap storage
   (m_capacityAndFlags >= 0). The reference geometry pointers and breakable
   shape / triangle map are not owned here. */
void hkdAssetProcessingUtil_BaseObjectSource_destruct(hkdAssetProcessingUtil_BaseObjectSource *self)
{
    hkThreadMemory *mem = hkThreadMemory_getCurrent();
    int cap;

    cap = self->m_numTrianglesPerSection.m_capacityAndFlags;
    if (cap >= 0)
        hkThreadMemory_deallocateChunk(mem, self->m_numTrianglesPerSection.m_data,
                                       4 * cap, HK_MEMORY_CLASS_ARRAY);

    cap = self->m_numVerticesPerSection.m_capacityAndFlags;
    if (cap >= 0)
        hkThreadMemory_deallocateChunk(mem, self->m_numVerticesPerSection.m_data,
                                       4 * cap, HK_MEMORY_CLASS_ARRAY);
}
