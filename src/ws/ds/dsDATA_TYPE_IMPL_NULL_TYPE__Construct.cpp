#include "dsDATA_TYPE_IMPL.h"
#include "NULL_TYPE.h"

// ds_data::dsDATA_TYPE_IMPL<NULL_TYPE>::Construct @ 0x825A7BF0
// NULL_TYPE carries no storage; construction is a no-op (function body is a bare blr).
template<>
void ds_data::dsDATA_TYPE_IMPL<NULL_TYPE>::Construct(unsigned int *storage) const
{
    (void)storage;
}
