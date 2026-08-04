#include "../headers/ws/ds/ds_boundary.h"
#include "../headers/ws/ds/dsBIT_STREAM.h"
#include "../headers/ws/ds/dsDATA.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"

// ds_data::dsDATA_TYPE_STATIC<haloPLAYER_ACTIONS_ENV>::NetPack @ 0x823BDB08
// Thin forwarder to the stored type's own Pack (network serialize).
template<>
void ds_data::dsDATA_TYPE_STATIC<haloPLAYER_ACTIONS_ENV>::NetPack(
        const haloPLAYER_ACTIONS_ENV &data, ds::BIT_STREAM &stream, const dsDATA &userData)
{
    data.Pack(stream, userData);
}
