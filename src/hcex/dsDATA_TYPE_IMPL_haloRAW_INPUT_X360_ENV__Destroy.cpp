#include "../headers/ws/ds/dsDATA_TYPE_IMPL.h"
#include "../headers/hcex/halo_ds_data_type_boundary.h"
#include "../headers/hcex/haloRAW_INPUT_X360_ENV.h"

// ds_data::dsDATA_TYPE_IMPL<haloRAW_INPUT_X360_ENV>::Destroy @ 0x823BF088
// Virtual override. Forward to the construction-policy twin (frees the boxed value, nulls slot).
template<>
void ds_data::dsDATA_TYPE_IMPL<haloRAW_INPUT_X360_ENV>::Destroy(unsigned int *storage) const
{
    ds_data::dsDATA_TYPE_CONSTRUCT<haloRAW_INPUT_X360_ENV>::Destroy(storage);
}
