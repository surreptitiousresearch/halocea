#include "dsDATA_TYPE_IMPL.h"
#include "ds_boundary.h"

// ds_data::dsDATA_TYPE_IMPL<int>::Construct @ 0x825A7D28
// Default-construct the int slot from the type's static default value.
template<>
void ds_data::dsDATA_TYPE_IMPL<int>::Construct(unsigned int *storage) const
{
    if (storage)
        *(int *)storage = ds_data::dsDATA_DEFAULT_VALUE<int>::GetDefaultVal();
}
