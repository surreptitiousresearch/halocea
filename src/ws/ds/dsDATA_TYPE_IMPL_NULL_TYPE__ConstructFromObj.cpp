#include "dsDATA_TYPE_IMPL.h"
#include "NULL_TYPE.h"

// ds_data::dsDATA_TYPE_IMPL<NULL_TYPE>::ConstructFromObj @ 0x825A7C00
// NULL_TYPE carries no storage; copy-construction is a no-op (function body is a bare blr).
template<>
void ds_data::dsDATA_TYPE_IMPL<NULL_TYPE>::ConstructFromObj(unsigned int *storage, const void *obj) const
{
    (void)storage;
    (void)obj;
}
