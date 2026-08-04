#include "dsDATA_TYPE_IMPL.h"

// Placement/debug global allocator the ws build links in: operator new(size, file, line).
extern void *operator new(size_t size, const char *file, unsigned int line);

// ds_data::dsDATA_TYPE_IMPL<double>::ConstructFromObj @ 0x825A7EB8
// A double does not fit the inline 4-byte slot, so it is boxed on the heap and the slot
// holds the pointer. Copy the source double into a freshly allocated box.
// Deviation: the decompiler carried the value as __int64 through fp0; it is a double copy.
template<>
void ds_data::dsDATA_TYPE_IMPL<double>::ConstructFromObj(unsigned int *storage, const void *obj) const
{
    double *box = (double *)operator new(sizeof(double),
        "D:\\Projects\\code\\common\\incl.sys\\ds\\ds_data_type_impl.hpp", 0x225u);
    if (box) {
        *box = *(const double *)obj;
        *storage = (unsigned int)box;
    } else {
        *storage = 0;
    }
}
