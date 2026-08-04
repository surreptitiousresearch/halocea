#include "../headers/ws/ds/dsDATA_TYPE_IMPL.h"
#include "../headers/hcex/halo_ds_data_type_boundary.h"
#include "../headers/hcex/haloRAW_INPUT_X360_ENV.h"

// ds_data::dsDATA_TYPE_IMPL<haloRAW_INPUT_X360_ENV>::Copy @ 0x823BF0B8
// Virtual override. Forward to the construction-policy twin (overwrite dest from src).
template<>
void ds_data::dsDATA_TYPE_IMPL<haloRAW_INPUT_X360_ENV>::Copy(
        const unsigned int *src, unsigned int *dest) const
{
    ds_data::dsDATA_TYPE_CONSTRUCT<haloRAW_INPUT_X360_ENV>::Copy(src, dest);
}
