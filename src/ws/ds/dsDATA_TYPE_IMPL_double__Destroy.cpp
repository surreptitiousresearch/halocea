#include "dsDATA_TYPE_IMPL.h"
#include <new>

// ds_data::dsDATA_TYPE_IMPL<double>::Destroy @ 0x8257FAE8
// The double slot holds a heap-allocated value; free it and null the slot.
template<>
void ds_data::dsDATA_TYPE_IMPL<double>::Destroy(unsigned int *storage) const
{
    void **heapSlot = (void **)storage;
    operator delete(*heapSlot);
    *heapSlot = nullptr;
}
