#include "../headers/ws/ds/ds_boundary.h"
#include "../headers/hcex/halo_ds_data_type_boundary.h"
#include "../headers/hcex/haloRAW_INPUT_X360_ENV.h"

// ds_data::dsDATA_TYPE_STATIC<haloRAW_INPUT_X360_ENV>::CopyConstruct @ 0x823BF730
// Thin forwarder: construct a fresh slot value from an existing slot value.
template<>
void ds_data::dsDATA_TYPE_STATIC<haloRAW_INPUT_X360_ENV>::CopyConstruct(unsigned int *storage, const unsigned int *data)
{
    ds_data::dsDATA_TYPE_CONSTRUCT<haloRAW_INPUT_X360_ENV>::CopyConstruct(storage, data);
}
