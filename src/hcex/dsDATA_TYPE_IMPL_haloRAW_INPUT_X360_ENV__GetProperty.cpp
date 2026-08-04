#include "../headers/ws/ds/dsDATA_TYPE_IMPL.h"
#include "../headers/hcex/halo_ds_data_type_boundary.h"
#include "../headers/ws/ds/dsSTRID.h"
#include "../headers/ws/ds/dsDATA.h"
#include "../headers/hcex/haloRAW_INPUT_X360_ENV.h"

// ds_data::dsDATA_TYPE_IMPL<haloRAW_INPUT_X360_ENV>::GetProperty @ 0x823BD6D8
// Thin virtual override: forward to the named-property policy for the stored type.
// The mangled name passes dsSTRID by value and dsDATA by reference; the PPC decompiler renders
// these as pointers (by-value class args are passed by hidden pointer on this ABI).
template<>
int ds_data::dsDATA_TYPE_IMPL<haloRAW_INPUT_X360_ENV>::GetProperty(
        dsSTRID id, const dsDATA &inst, dsDATA &val) const
{
    return ds_data::dsDATA_TYPE_CLASS<haloRAW_INPUT_X360_ENV>::GetProperty(id, inst, val);
}
