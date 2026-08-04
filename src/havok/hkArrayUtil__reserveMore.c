#include "../headers/havok/hkThreadMemory.h"

/* Raw hkArray header view (matches hkArray<T>: data ptr, size, capacityAndFlags).
   hkArrayUtil operates on any element type, so it takes the array generically. */
struct hkArrayHeader
{
    void *m_data;             /* 0x00 */
    int   m_size;             /* 0x04 */
    int   m_capacityAndFlags; /* 0x08 */
};

extern void *memcpy(void *dst, const void *src, unsigned int n);

/* hkArrayUtil::_reserveMore — grow an array so it can hold more elements:
   allocate a new buffer of 2*m_size elements (1 if empty), copy the existing
   m_size elements, release the old buffer if it was heap-owned, and update the
   capacity while preserving the "locked" flag (0x40000000) and clearing the
   "does not own storage" sign bit. */
/* @ 0x838E0C98, ?_reserveMore@hkArrayUtil@@YAXPAXH@Z: arg0 void* per PAX */
void hkArrayUtil__reserveMore(void *array, int sizeElem)
{
    struct hkArrayHeader *hdr = (struct hkArrayHeader *)array;
    hkThreadMemory *tm = hkThreadMemory_getCurrent();
    int newCapacity = 2 * hdr->m_size;
    int oldCapAndFlags;
    void *chunk;

    if (!hdr->m_size)
        newCapacity = 1;

    chunk = hkThreadMemory_allocateChunk(tm, newCapacity * sizeElem, HK_MEMORY_CLASS_ARRAY);
    memcpy(chunk, hdr->m_data, hdr->m_size * sizeElem);

    oldCapAndFlags = hdr->m_capacityAndFlags;
    if (oldCapAndFlags >= 0) /* sign bit clear => this buffer was heap-allocated */
        hkThreadMemory_deallocateChunk(tm, hdr->m_data,
                                       (oldCapAndFlags & 0x3FFFFFFF) * sizeElem,
                                       HK_MEMORY_CLASS_ARRAY);

    hdr->m_data = chunk;
    hdr->m_capacityAndFlags = (oldCapAndFlags & 0x40000000) | newCapacity;
}
