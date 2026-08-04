#include "../headers/ws/ds/ds_boundary.h"
#include "../headers/ws/ds/dsDATA_TYPE.h"
#include "../headers/hcex/halo_ds_data_type_boundary.h"
#include "../headers/hcex/haloRAW_INPUT_X360_ENV.h"

// ds_data::dsDATA_TYPE_STATIC<haloRAW_INPUT_X360_ENV>::MTSafeAnalog @ 0x823BDE58
// Return the MT-safe type analog for this type (id 0).
template<>
const dsDATA_TYPE *ds_data::dsDATA_TYPE_STATIC<haloRAW_INPUT_X360_ENV>::MTSafeAnalog()
{
    return ds_data::GetMTSafeTypeAnalog(0);
}
