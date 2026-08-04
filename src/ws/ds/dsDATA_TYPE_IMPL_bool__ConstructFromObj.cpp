#include "dsDATA_TYPE_IMPL.h"

// ds_data::dsDATA_TYPE_IMPL<bool>::ConstructFromObj @ 0x825A7DE8
// Copy the bool value from a source object into the inline slot (skips a null slot).
template<>
void ds_data::dsDATA_TYPE_IMPL<bool>::ConstructFromObj(unsigned int *storage, const void *obj) const
{
    if (storage)
        *(unsigned char *)storage = *(const unsigned char *)obj;
}
