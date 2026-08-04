#include "dsDATA_TYPE_IMPL.h"
#include "ds_boundary.h"

// Placement/debug global allocator the ws build links in: operator new(size, file, line).
extern void *operator new(size_t size, const char *file, unsigned int line);

// ds_data::dsDATA_TYPE_STATIC<double>::Construct @ 0x82580240
// A double does not fit the inline 4-byte slot, so it is boxed on the heap and the slot holds
// the pointer. Initialise the box with the type's default value.
// Deviation: the decompiler carried the default through a __int64; the disasm is `lfd; stfd`,
// i.e. a double load/store of the static default instance.
template<>
void ds_data::dsDATA_TYPE_STATIC<double>::Construct(unsigned int *storage)
{
    double *box = (double *)operator new(sizeof(double),
        "D:\\Projects\\code\\common\\incl.sys\\ds\\ds_data_type_impl.hpp", 0x225u);
    if (box) {
        *storage = (unsigned int)box;
        *box = ds_data::dsDATA_DEFAULT_VALUE<double>::GetDefaultVal();
    } else {
        *storage = 0;
    }
}
