#include "../headers/ws/ds/dsDATA_TYPE_IMPL.h"
#include "../headers/hcex/halo_ds_data_type_boundary.h"
#include "../headers/hcex/haloRAW_INPUT_X360_ENV.h"

// ds_data::dsDATA_TYPE_IMPL<haloRAW_INPUT_X360_ENV>::IsMTSafe @ 0x823BD768
// Touch the stored value's pointer (side-effect-free access under the construct policy) and
// report not-MT-safe (0). The GetPtr result is intentionally discarded, mirroring the disasm.
template<>
int ds_data::dsDATA_TYPE_IMPL<haloRAW_INPUT_X360_ENV>::IsMTSafe(const unsigned int *storage) const
{
    ds_data::dsDATA_TYPE_CONSTRUCT<haloRAW_INPUT_X360_ENV>::GetPtr(storage);
    return 0;
}
