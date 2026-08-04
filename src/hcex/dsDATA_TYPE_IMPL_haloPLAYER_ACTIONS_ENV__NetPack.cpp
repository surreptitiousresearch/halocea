#include "../headers/ws/ds/dsDATA_TYPE_IMPL.h"
#include "../headers/ws/ds/dsDATA.h"
#include "../headers/hcex/halo_ds_data_type_boundary.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"

// ds_data::dsDATA_TYPE_IMPL<haloPLAYER_ACTIONS_ENV>::NetPack @ 0x823BEEF0
// Virtual override. Resolve the stored value and forward to its own bit-stream serializer.
template<>
void ds_data::dsDATA_TYPE_IMPL<haloPLAYER_ACTIONS_ENV>::NetPack(
        const unsigned int *storage, ds::BIT_STREAM &stream, const dsDATA &userData) const
{
    const haloPLAYER_ACTIONS_ENV *value =
        ds_data::dsDATA_TYPE_CONSTRUCT<haloPLAYER_ACTIONS_ENV>::GetPtr(storage);
    value->Pack(stream, userData);
}
