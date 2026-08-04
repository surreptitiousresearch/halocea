#include "../headers/ws/ds/dsDATA_TYPE_IMPL.h"
#include "../headers/hcex/halo_ds_data_type_boundary.h"
#include "../headers/ws/ds/dsSTRID.h"
#include "../headers/ws/ds/dsDATA.h"
#include "../headers/hcex/haloRAW_INPUT_X360_ENV.h"

// ds_data::dsDATA_TYPE_IMPL<haloRAW_INPUT_X360_ENV>::SetProperty @ 0x823BD718
// Thin virtual override: forward to the named-property policy for the stored type.
template<>
int ds_data::dsDATA_TYPE_IMPL<haloRAW_INPUT_X360_ENV>::SetProperty(
        dsSTRID id, dsDATA &inst, const dsDATA &val) const
{
    return ds_data::dsDATA_TYPE_CLASS<haloRAW_INPUT_X360_ENV>::SetProperty(id, inst, val);
}
