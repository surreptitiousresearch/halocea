#include "../headers/ws/ds/ds_boundary.h"
#include "../headers/hcex/halo_ds_data_type_boundary.h"
#include "../headers/hcex/haloRAW_INPUT_X360_ENV.h"

// ds_data::dsDATA_TYPE_STATIC<haloRAW_INPUT_X360_ENV>::Construct @ 0x823BF700
// Thin forwarder: construct a fresh slot value from the type's function-static default instance.
template<>
void ds_data::dsDATA_TYPE_STATIC<haloRAW_INPUT_X360_ENV>::Construct(unsigned int *storage)
{
    ds_data::dsDATA_TYPE_CONSTRUCT<haloRAW_INPUT_X360_ENV>::Construct(
        storage, &ds_data::dsDATA_DEFAULT_VALUE<haloRAW_INPUT_X360_ENV>::GetDefaultVal());
}
