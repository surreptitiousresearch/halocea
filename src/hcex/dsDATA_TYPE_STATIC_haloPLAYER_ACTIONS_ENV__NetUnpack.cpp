#include "../headers/ws/ds/ds_boundary.h"
#include "../headers/ws/ds/dsBIT_STREAM.h"
#include "../headers/ws/ds/dsDATA.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"

// ds_data::dsDATA_TYPE_STATIC<haloPLAYER_ACTIONS_ENV>::NetUnpack @ 0x823BDB40
// Thin forwarder to the stored type's own Unpack (network deserialize).
template<>
void ds_data::dsDATA_TYPE_STATIC<haloPLAYER_ACTIONS_ENV>::NetUnpack(
        ds::BIT_STREAM &stream, haloPLAYER_ACTIONS_ENV &data, const dsDATA &userData)
{
    data.Unpack(stream, userData);
}
