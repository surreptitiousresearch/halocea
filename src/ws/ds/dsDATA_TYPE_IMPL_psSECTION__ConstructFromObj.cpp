#include "dsDATA_TYPE_IMPL.h"

struct psSECTION; // boundary — ps-subsystem refcounted section, opaque here

// ds_data::dsDATA_TYPE_IMPL<psSECTION>::ConstructFromObj @ 0x82518270
// Store the source handle and add references to both the section and the buffer it owns
// (each an intrusive refcount at offset 0; the owned buffer pointer lives at offset 0x34).
template<>
void ds_data::dsDATA_TYPE_IMPL<psSECTION>::ConstructFromObj(unsigned int *storage, const void *obj) const
{
    if (storage)
    {
        psSECTION *src = *(psSECTION *const *)obj;
        if (src)
        {
            int *ownedBuffer = *reinterpret_cast<int **>(reinterpret_cast<char *>(src) + 0x34); // owned buffer ptr @0x34
            ++*reinterpret_cast<int *>(src); // section refcount @0
            ++*ownedBuffer;                  // owned-buffer refcount @0
        }
        *(psSECTION **)storage = src;
    }
}
