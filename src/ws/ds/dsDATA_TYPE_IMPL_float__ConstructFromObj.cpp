#include "dsDATA_TYPE_IMPL.h"

// ds_data::dsDATA_TYPE_IMPL<float>::ConstructFromObj @ 0x825A7E30
// Copy the float value from a source object into the inline slot (skips a null slot).
template<>
void ds_data::dsDATA_TYPE_IMPL<float>::ConstructFromObj(unsigned int *storage, const void *obj) const
{
    if (storage)
        *storage = *(const unsigned int *)obj; // 4-byte copy of the stored float
}
