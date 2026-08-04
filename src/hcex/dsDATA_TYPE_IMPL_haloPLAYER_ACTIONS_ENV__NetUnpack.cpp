#include "../headers/ws/ds/dsDATA_TYPE_IMPL.h"
#include "../headers/ws/ds/dsDATA.h"
#include "../headers/hcex/halo_ds_data_type_boundary.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"

// ds_data::dsDATA_TYPE_IMPL<haloPLAYER_ACTIONS_ENV>::NetUnpack @ 0x823BEF38
// Virtual override. Resolve the destination slot and forward to the value's own deserializer.
template<>
void ds_data::dsDATA_TYPE_IMPL<haloPLAYER_ACTIONS_ENV>::NetUnpack(
        ds::BIT_STREAM &stream, unsigned int *storage, const dsDATA &userData) const
{
    haloPLAYER_ACTIONS_ENV *value =
        ds_data::dsDATA_TYPE_CONSTRUCT<haloPLAYER_ACTIONS_ENV>::GetPtr(storage);
    value->Unpack(stream, userData);
}
