#include "../headers/ws/ds/dsDATA_TYPE_IMPL.h"
#include "../headers/hcex/halo_ds_data_type_boundary.h"
#include "../headers/hcex/haloRAW_INPUT_X360_ENV.h"

// ds_data::dsDATA_TYPE_IMPL<haloRAW_INPUT_X360_ENV>::GetPtr @ 0x823BF128
// Thin virtual override: return the stored value's address via the construct policy.
template<>
void *ds_data::dsDATA_TYPE_IMPL<haloRAW_INPUT_X360_ENV>::GetPtr(unsigned int *storage) const
{
    return ds_data::dsDATA_TYPE_CONSTRUCT<haloRAW_INPUT_X360_ENV>::GetPtr(storage);
}
