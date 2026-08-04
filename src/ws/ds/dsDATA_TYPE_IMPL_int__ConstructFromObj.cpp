#include "dsDATA_TYPE_IMPL.h"

// ds_data::dsDATA_TYPE_IMPL<int>::ConstructFromObj @ 0x825A7D58
// Copy the int value from a source object into the inline slot (skips a null slot).
template<>
void ds_data::dsDATA_TYPE_IMPL<int>::ConstructFromObj(unsigned int *storage, const void *obj) const
{
    if (storage)
        *storage = *(const unsigned int *)obj;
}
