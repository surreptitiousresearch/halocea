#include "dsDATA_TYPE_IMPL.h"

// ds_data::dsDATA_TYPE_IMPL<int>::GetPtr @ 0x8257F1F0
// int is stored inline in the 4-byte slot; return the slot address itself.
// Return type is void* per the mangle (PAX); the decompiler narrowed it to unsigned int*.
template<>
void *ds_data::dsDATA_TYPE_IMPL<int>::GetPtr(unsigned int *storage) const
{
    return storage;
}
