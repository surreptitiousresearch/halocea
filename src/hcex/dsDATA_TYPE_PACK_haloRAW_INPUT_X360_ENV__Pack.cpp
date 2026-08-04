#include "../headers/ws/ds/dsDATA_TYPE_PACK.h"
#include "../headers/hcex/haloRAW_INPUT_X360_ENV.h"

// ds_data::dsDATA_TYPE_PACK<haloRAW_INPUT_X360_ENV>::Pack @ 0x823BD5A8
// Forward to the type's own member serializer.
template<>
void ds_data::dsDATA_TYPE_PACK<haloRAW_INPUT_X360_ENV>::Pack(
        const haloRAW_INPUT_X360_ENV &data, ds::BIT_STREAM &stream, const dsDATA &userData)
{
    data.Pack(stream, userData);
}
