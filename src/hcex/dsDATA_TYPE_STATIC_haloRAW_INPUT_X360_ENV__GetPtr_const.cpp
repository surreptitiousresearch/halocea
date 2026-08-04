#include "../headers/ws/ds/ds_boundary.h"
#include "../headers/hcex/halo_ds_data_type_boundary.h"
#include "../headers/hcex/haloRAW_INPUT_X360_ENV.h"

// ds_data::dsDATA_TYPE_STATIC<haloRAW_INPUT_X360_ENV>::GetPtr(const) @ 0x823BDCE8
// Thin forwarder returning a const pointer into the stored slot.
template<>
const haloRAW_INPUT_X360_ENV *ds_data::dsDATA_TYPE_STATIC<haloRAW_INPUT_X360_ENV>::GetPtr(const unsigned int *storage)
{
    return ds_data::dsDATA_TYPE_CONSTRUCT<haloRAW_INPUT_X360_ENV>::GetPtr(storage);
}
