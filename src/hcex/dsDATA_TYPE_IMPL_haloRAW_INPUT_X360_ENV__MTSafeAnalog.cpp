#include "../headers/ws/ds/dsDATA_TYPE_IMPL.h"
#include "../headers/hcex/halo_ds_data_type_boundary.h"
#include "../headers/hcex/haloRAW_INPUT_X360_ENV.h"

// ds_data::dsDATA_TYPE_IMPL<haloRAW_INPUT_X360_ENV>::MTSafeAnalog @ 0x823BF2C8
// Return this type's MT-safe replacement descriptor (typeId 0 -> this null/untyped instance).
template<>
const dsDATA_TYPE *ds_data::dsDATA_TYPE_IMPL<haloRAW_INPUT_X360_ENV>::MTSafeAnalog() const
{
    return ds_data::GetMTSafeTypeAnalog(0);
}
