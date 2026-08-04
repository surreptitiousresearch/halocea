#include "../headers/ws/ds/ds_boundary.h"
#include "../headers/hcex/halo_ds_data_type_boundary.h"
#include "../headers/hcex/haloRAW_INPUT_X360_ENV.h"

// ds_data::dsDATA_TYPE_STATIC<haloRAW_INPUT_X360_ENV>::ConstructFromObj @ 0x823BF760
// Thin forwarder: construct a fresh slot value from a raw object.
template<>
void ds_data::dsDATA_TYPE_STATIC<haloRAW_INPUT_X360_ENV>::ConstructFromObj(unsigned int *storage, const haloRAW_INPUT_X360_ENV *obj)
{
    ds_data::dsDATA_TYPE_CONSTRUCT<haloRAW_INPUT_X360_ENV>::Construct(storage, obj);
}
