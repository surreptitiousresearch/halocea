#include "../headers/ws/ds/dsDATA_TYPE_IMPL.h"
#include "../headers/hcex/halo_ds_data_type_boundary.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"

// ds_data::dsDATA_TYPE_IMPL<haloPLAYER_ACTIONS_ENV>::GetPtr (const storage) @ 0x823BEBE8
// Virtual override. Mangled return is const void* (PBX); forwards to the const-storage
// construction-policy twin.
template<>
const void *ds_data::dsDATA_TYPE_IMPL<haloPLAYER_ACTIONS_ENV>::GetPtr(const unsigned int *storage) const
{
    return ds_data::dsDATA_TYPE_CONSTRUCT<haloPLAYER_ACTIONS_ENV>::GetPtr(storage);
}
