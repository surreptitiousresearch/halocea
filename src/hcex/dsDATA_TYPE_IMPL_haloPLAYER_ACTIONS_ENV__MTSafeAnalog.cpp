#include "../headers/ws/ds/dsDATA_TYPE_IMPL.h"
#include "../headers/hcex/halo_ds_data_type_boundary.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"

// ds_data::dsDATA_TYPE_IMPL<haloPLAYER_ACTIONS_ENV>::MTSafeAnalog @ 0x823BED58
// Virtual override. Return the MT-safe type analog for this type (id 0).
template<>
const dsDATA_TYPE *ds_data::dsDATA_TYPE_IMPL<haloPLAYER_ACTIONS_ENV>::MTSafeAnalog() const
{
    return ds_data::GetMTSafeTypeAnalog(0);
}
