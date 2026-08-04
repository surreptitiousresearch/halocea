#include "../headers/ws/ds/ds_boundary.h"
#include "../headers/hcex/haloRAW_INPUT_X360_ENV.h"
#include "../headers/ws/ds/dsDATA.h"

// ds_data::dsDATA_TYPE_STATIC<haloRAW_INPUT_X360_ENV>::NetPack @ 0x823BDFC0
// Thin forwarder: serialize the value through the stored type's own Pack member.
template<>
void ds_data::dsDATA_TYPE_STATIC<haloRAW_INPUT_X360_ENV>::NetPack(
        const haloRAW_INPUT_X360_ENV &data, ds::BIT_STREAM &stream, const dsDATA &userData)
{
    data.Pack(stream, userData);
}
