#include "../headers/ws/ds/ds_boundary.h"
#include "../headers/hcex/halo_ds_data_type_boundary.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"

// ds_data::dsDATA_TYPE_STATIC<haloPLAYER_ACTIONS_ENV>::ConstructFromObj @ 0x823BF6D0
// Thin forwarder: construct a fresh slot value from a raw object.
template<>
void ds_data::dsDATA_TYPE_STATIC<haloPLAYER_ACTIONS_ENV>::ConstructFromObj(unsigned int *storage, const haloPLAYER_ACTIONS_ENV *obj)
{
    ds_data::dsDATA_TYPE_CONSTRUCT<haloPLAYER_ACTIONS_ENV>::Construct(storage, obj);
}
