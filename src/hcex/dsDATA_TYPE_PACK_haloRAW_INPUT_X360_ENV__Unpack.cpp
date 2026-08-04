#include "../headers/ws/ds/dsDATA_TYPE_PACK.h"
#include "../headers/hcex/haloRAW_INPUT_X360_ENV.h"

// ds_data::dsDATA_TYPE_PACK<haloRAW_INPUT_X360_ENV>::Unpack @ 0x823BD5E0
// Forward to the type's own member deserializer.
template<>
void ds_data::dsDATA_TYPE_PACK<haloRAW_INPUT_X360_ENV>::Unpack(
        ds::BIT_STREAM &stream, haloRAW_INPUT_X360_ENV &data, const dsDATA &userData)
{
    data.Unpack(stream, userData);
}
