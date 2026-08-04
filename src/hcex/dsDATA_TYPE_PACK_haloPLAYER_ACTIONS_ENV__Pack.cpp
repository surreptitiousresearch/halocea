#include "../headers/ws/ds/dsDATA_TYPE_HELPERS.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"

// ds_data::dsDATA_TYPE_PACK<haloPLAYER_ACTIONS_ENV>::Pack() @ 0x823BC8F8
// Thin forwarder to the value type's own const Pack member.
template<>
void ds_data::dsDATA_TYPE_PACK<haloPLAYER_ACTIONS_ENV>::Pack(
    const haloPLAYER_ACTIONS_ENV &data, ds::BIT_STREAM &stream, const dsDATA &userData)
{
    data.Pack(stream, userData);
}
