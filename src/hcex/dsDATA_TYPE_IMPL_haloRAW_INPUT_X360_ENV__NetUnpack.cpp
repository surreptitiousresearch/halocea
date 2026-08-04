#include "../headers/ws/ds/dsDATA_TYPE_IMPL.h"
#include "../headers/hcex/halo_ds_data_type_boundary.h"
#include "../headers/hcex/haloRAW_INPUT_X360_ENV.h"

// ds_data::dsDATA_TYPE_IMPL<haloRAW_INPUT_X360_ENV>::NetUnpack @ 0x823BF4A8
// Resolve the (mutable) stored value and deserialize into it via its own Unpack.
// Mangled passes the stream/userData by reference (AAV/ABV); the PPC decompiler renders pointers.
template<>
void ds_data::dsDATA_TYPE_IMPL<haloRAW_INPUT_X360_ENV>::NetUnpack(
        ds::BIT_STREAM &stream, unsigned int *storage, const dsDATA &userData) const
{
    haloRAW_INPUT_X360_ENV *value =
        ds_data::dsDATA_TYPE_CONSTRUCT<haloRAW_INPUT_X360_ENV>::GetPtr(storage);
    value->Unpack(stream, userData);
}
