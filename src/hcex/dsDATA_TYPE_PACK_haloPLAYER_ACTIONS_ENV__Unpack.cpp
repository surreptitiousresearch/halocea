#include "../headers/ws/ds/dsDATA_TYPE_HELPERS.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"

// ds_data::dsDATA_TYPE_PACK<haloPLAYER_ACTIONS_ENV>::Unpack() @ 0x823BC930
// Thin forwarder to the value type's own Unpack member.
template<>
void ds_data::dsDATA_TYPE_PACK<haloPLAYER_ACTIONS_ENV>::Unpack(
    ds::BIT_STREAM &stream, haloPLAYER_ACTIONS_ENV &data, const dsDATA &userData)
{
    data.Unpack(stream, userData);
}
