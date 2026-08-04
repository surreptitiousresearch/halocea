#include "../headers/ws/ds/dsDATA_TYPE_IMPL.h"
#include "../headers/hcex/halo_ds_data_type_boundary.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"

// ds_data::dsDATA_TYPE_IMPL<haloPLAYER_ACTIONS_ENV>::ConstructFromObj @ 0x823BFC90
// Virtual override: construct a fresh slot value from a raw object. The base virtual takes a
// `const void*`; forward it as the concrete element type to the construct policy.
template<>
void ds_data::dsDATA_TYPE_IMPL<haloPLAYER_ACTIONS_ENV>::ConstructFromObj(unsigned int *storage, const void *obj) const
{
    ds_data::dsDATA_TYPE_CONSTRUCT<haloPLAYER_ACTIONS_ENV>::Construct(
        storage, static_cast<const haloPLAYER_ACTIONS_ENV *>(obj));
}
