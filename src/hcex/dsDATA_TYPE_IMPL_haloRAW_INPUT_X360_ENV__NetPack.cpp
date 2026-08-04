#include "../headers/ws/ds/dsDATA_TYPE_IMPL.h"
#include "../headers/hcex/halo_ds_data_type_boundary.h"
#include "../headers/hcex/haloRAW_INPUT_X360_ENV.h"

// ds_data::dsDATA_TYPE_IMPL<haloRAW_INPUT_X360_ENV>::NetPack @ 0x823BF460
// Resolve the stored value and serialize it into the bit stream via its own Pack.
// Mangled passes the stream/userData by reference (AAV/ABV); the PPC decompiler renders pointers.
template<>
void ds_data::dsDATA_TYPE_IMPL<haloRAW_INPUT_X360_ENV>::NetPack(
        const unsigned int *storage, ds::BIT_STREAM &stream, const dsDATA &userData) const
{
    // const storage selects the const GetPtr overload; Pack is a const method.
    const haloRAW_INPUT_X360_ENV *value =
        ds_data::dsDATA_TYPE_CONSTRUCT<haloRAW_INPUT_X360_ENV>::GetPtr(storage);
    value->Pack(stream, userData);
}
