#include "dsDATA_TYPE_IMPL.h"

struct psARRAY; // boundary — ps-subsystem refcounted array, opaque here

// ds_data::dsDATA_TYPE_IMPL<psARRAY>::ConstructFromObj @ 0x827B1680
// Store the source handle and add an intrusive reference (refcount at offset 0).
template<>
void ds_data::dsDATA_TYPE_IMPL<psARRAY>::ConstructFromObj(unsigned int *storage, const void *obj) const
{
    if (storage)
    {
        psARRAY *src = *(psARRAY *const *)obj;
        *(psARRAY **)storage = src;
        if (src)
            ++*reinterpret_cast<int *>(src); // intrusive refcount @0 (opaque boundary)
    }
}
