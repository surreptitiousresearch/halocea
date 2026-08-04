#include "dsDATA_TYPE_IMPL.h"
#include "dsTSTRING.h"

// ds_data::dsDATA_TYPE_IMPL<dsTSTRING<char>>::ConstructFromObj @ 0x825AB180
// Copy-construct the string slot: share the source buffer and bump its refcount.
template<>
void ds_data::dsDATA_TYPE_IMPL<dsTSTRING<char> >::ConstructFromObj(unsigned int *storage, const void *obj) const
{
    if (storage)
    {
        dsTSTRING<char>       *dest = (dsTSTRING<char> *)storage;
        const dsTSTRING<char> *src  = (const dsTSTRING<char> *)obj;
        dest->pBuffer = nullptr;
        dest->pBuffer = src->pBuffer;
        ++dest->pBuffer->refCount;
    }
}
