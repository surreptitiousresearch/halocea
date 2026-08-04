#include "dsDATA_TYPE_IMPL.h"
#include "ds_boundary.h"

// ds_data::dsDATA_TYPE_IMPL<double>::Construct @ 0x825A7E48
// Default-construct the double slot by forwarding to the static policy.
template<>
void ds_data::dsDATA_TYPE_IMPL<double>::Construct(unsigned int *storage) const
{
    ds_data::dsDATA_TYPE_STATIC<double>::Construct(storage);
}
