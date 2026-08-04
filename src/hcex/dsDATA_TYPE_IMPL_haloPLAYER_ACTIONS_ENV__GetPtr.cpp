#include "../headers/ws/ds/dsDATA_TYPE_IMPL.h"
#include "../headers/hcex/halo_ds_data_type_boundary.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"

// ds_data::dsDATA_TYPE_IMPL<haloPLAYER_ACTIONS_ENV>::GetPtr @ 0x823BEBB8
// Virtual override (mutable storage). Mangled return is void* (PAX); forwards to the
// construction-policy twin, whose T* result converts to void*.
template<>
void *ds_data::dsDATA_TYPE_IMPL<haloPLAYER_ACTIONS_ENV>::GetPtr(unsigned int *storage) const
{
    return ds_data::dsDATA_TYPE_CONSTRUCT<haloPLAYER_ACTIONS_ENV>::GetPtr(storage);
}
