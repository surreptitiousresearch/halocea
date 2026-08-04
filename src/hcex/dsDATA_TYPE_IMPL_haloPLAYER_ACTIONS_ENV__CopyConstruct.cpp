#include "../headers/ws/ds/dsDATA_TYPE_IMPL.h"
#include "../headers/hcex/halo_ds_data_type_boundary.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"

// ds_data::dsDATA_TYPE_IMPL<haloPLAYER_ACTIONS_ENV>::CopyConstruct @ 0x823BFC58
// Virtual override: construct a fresh slot value from an existing slot value.
template<>
void ds_data::dsDATA_TYPE_IMPL<haloPLAYER_ACTIONS_ENV>::CopyConstruct(unsigned int *storage, const unsigned int *data) const
{
    ds_data::dsDATA_TYPE_CONSTRUCT<haloPLAYER_ACTIONS_ENV>::CopyConstruct(storage, data);
}
