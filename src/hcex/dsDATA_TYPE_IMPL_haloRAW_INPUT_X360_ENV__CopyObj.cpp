#include "../headers/ws/ds/dsDATA_TYPE_IMPL.h"
#include "../headers/hcex/halo_ds_data_type_boundary.h"
#include "../headers/hcex/haloRAW_INPUT_X360_ENV.h"

// ds_data::dsDATA_TYPE_IMPL<haloRAW_INPUT_X360_ENV>::CopyObj @ 0x823BF0F0
// Thin virtual override: assign the stored slot from `obj` via the construct policy.
// Mangled `obj` is const void* (PBX); the decompiler prints the concrete T* — same pointer.
template<>
void ds_data::dsDATA_TYPE_IMPL<haloRAW_INPUT_X360_ENV>::CopyObj(
        const void *obj, unsigned int *dest) const
{
    ds_data::dsDATA_TYPE_CONSTRUCT<haloRAW_INPUT_X360_ENV>::CopyObj(
        static_cast<const haloRAW_INPUT_X360_ENV *>(obj), dest);
}
