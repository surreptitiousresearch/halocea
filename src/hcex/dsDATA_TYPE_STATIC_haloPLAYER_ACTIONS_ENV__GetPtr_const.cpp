#include "../headers/ws/ds/ds_boundary.h"
#include "../headers/hcex/halo_ds_data_type_boundary.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"

// ds_data::dsDATA_TYPE_STATIC<haloPLAYER_ACTIONS_ENV>::GetPtr(const) @ 0x823BD830
// Thin forwarder returning a const pointer into the stored slot.
template<>
const haloPLAYER_ACTIONS_ENV *ds_data::dsDATA_TYPE_STATIC<haloPLAYER_ACTIONS_ENV>::GetPtr(const unsigned int *storage)
{
    return ds_data::dsDATA_TYPE_CONSTRUCT<haloPLAYER_ACTIONS_ENV>::GetPtr(storage);
}
