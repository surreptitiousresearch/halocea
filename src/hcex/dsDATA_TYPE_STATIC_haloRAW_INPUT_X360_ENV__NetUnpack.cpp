#include "../headers/ws/ds/ds_boundary.h"
#include "../headers/hcex/haloRAW_INPUT_X360_ENV.h"
#include "../headers/ws/ds/dsDATA.h"

// ds_data::dsDATA_TYPE_STATIC<haloRAW_INPUT_X360_ENV>::NetUnpack @ 0x823BDFF8
// Thin forwarder: deserialize the value through the stored type's own Unpack member.
template<>
void ds_data::dsDATA_TYPE_STATIC<haloRAW_INPUT_X360_ENV>::NetUnpack(
        ds::BIT_STREAM &stream, haloRAW_INPUT_X360_ENV &data, const dsDATA &userData)
{
    data.Unpack(stream, userData);
}
