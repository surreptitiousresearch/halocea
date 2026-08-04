#include "../headers/ws/ds/dsDATA_TYPE_IMPL.h"
#include "../headers/hcex/halo_ds_data_type_boundary.h"
#include "../headers/hcex/haloRAW_INPUT_X360_ENV.h"

// ds_data::dsDATA_TYPE_IMPL<haloRAW_INPUT_X360_ENV>::GetPtr(const) @ 0x823BF158
// Const overload of GetPtr: return a read-only pointer to the stored value.
template<>
const void *ds_data::dsDATA_TYPE_IMPL<haloRAW_INPUT_X360_ENV>::GetPtr(const unsigned int *storage) const
{
    return ds_data::dsDATA_TYPE_CONSTRUCT<haloRAW_INPUT_X360_ENV>::GetPtr(storage);
}
