#include <string.h>
#include "../headers/havok/hkThreadMemory.h"

/* Raw hkArray header view (matches hkArray<T>: data ptr, size, capacityAndFlags). */
struct hkArrayHeader
{
    void *m_data;             /* 0x00 */
    int   m_size;             /* 0x04 */
    int   m_capacityAndFlags; /* 0x08 */
};


/* hkArrayUtil::_reserve — reallocate an array's storage to hold exactly numElem
   elements: allocate the new buffer, copy the existing m_size elements, release
   the old buffer if it was heap-owned, and set the capacity while preserving the
   "locked" flag (0x40000000) and clearing the "does not own storage" sign bit.
   The caller supplies numElem already sized (m_size is left unchanged here). */
/* @ 0x838E0C10, ?_reserve@hkArrayUtil@@YAXPAXHH@Z: arg0 void* per PAX */
void hkArrayUtil__reserve(void *array, int numElem, int sizeElem)
{
    struct hkArrayHeader *hdr = (struct hkArrayHeader *)array;
    hkThreadMemory *tm = hkThreadMemory_getCurrent();
    int oldCapAndFlags;
    void *chunk;

    chunk = hkThreadMemory_allocateChunk(tm, numElem * sizeElem, HK_MEMORY_CLASS_ARRAY);
    memcpy(chunk, hdr->m_data, hdr->m_size * sizeElem);

    oldCapAndFlags = hdr->m_capacityAndFlags;
    if (oldCapAndFlags >= 0) /* sign bit clear => this buffer was heap-allocated */
        hkThreadMemory_deallocateChunk(tm, hdr->m_data,
                                       (oldCapAndFlags & 0x3FFFFFFF) * sizeElem,
                                       HK_MEMORY_CLASS_ARRAY);

    hdr->m_data = chunk;
    hdr->m_capacityAndFlags = (oldCapAndFlags & 0x40000000) | numElem;
}
