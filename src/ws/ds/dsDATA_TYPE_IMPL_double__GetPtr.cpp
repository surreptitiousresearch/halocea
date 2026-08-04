#include "dsDATA_TYPE_IMPL.h"

// ds_data::dsDATA_TYPE_IMPL<double>::GetPtr @ 0x8257FB48
// double is stored by pointer (it does not fit the 4-byte slot); the slot holds the
// address of the value, so dereference it. Return type is void* per the mangle (PAX).
template<>
void *ds_data::dsDATA_TYPE_IMPL<double>::GetPtr(unsigned int *storage) const
{
    return (void *)*storage;
}
