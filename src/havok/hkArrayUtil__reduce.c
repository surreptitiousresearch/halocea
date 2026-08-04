#include "../headers/havok/hkThreadMemory.h"

/* hkString::memCpy (boundary). */
extern void hkString_memCpy(void *dst, const void *src, int numBytes);

/* hkArrayUtil::_reduce — shrink an hkArray's backing store to requestedCapacity.
   `array` aliases the three raw hkArray words (m_data, m_size, capacityAndFlags).
   If an in-place buffer is supplied and the live element count is smaller than the
   requested capacity, the array is retargeted at it (flagged as don't-free,
   0x80000000); otherwise a right-sized chunk is allocated. The live elements are
   copied over, the old owned storage is released, and the capacity word is rebuilt
   preserving the locked bit (0x40000000).
   DEVIATION: the decompiler routes the allocator through the raw per-thread TLS
   deref; expressed here via hkThreadMemory_getCurrent(). */
/* @ 0x838E0D30, ?_reduce@hkArrayUtil@@YAXPAXHPADH@Z: arg0 void* per PAX */
void hkArrayUtil__reduce(void *arrayIn, int sizeElem, char *inplaceMem, int requestedCapacity)
{
    char **array = (char **)arrayIn;
    hkThreadMemory *mem = hkThreadMemory_getCurrent();
    int size = ((int *)array)[1];
    unsigned int oldCap = ((unsigned int *)array)[2];
    unsigned int dontFreeFlag = 0;
    char *newData;

    if (inplaceMem && size < requestedCapacity)
    {
        newData = inplaceMem;
        dontFreeFlag = 0x80000000u;
    }
    else
    {
        newData = (char *)hkThreadMemory_allocateChunk(mem, sizeElem * requestedCapacity,
                                                       HK_MEMORY_CLASS_ARRAY);
    }

    hkString_memCpy(newData, array[0], size * sizeElem);
    hkThreadMemory_deallocateChunk(mem, array[0], (oldCap & 0x3FFFFFFF) * sizeElem,
                                   HK_MEMORY_CLASS_ARRAY);
    array[0] = newData;
    ((unsigned int *)array)[2] = (oldCap & 0x40000000) | dontFreeFlag | (unsigned int)requestedCapacity;
}
