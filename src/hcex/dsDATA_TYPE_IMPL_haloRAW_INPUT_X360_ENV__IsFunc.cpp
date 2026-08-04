#include "../headers/ws/ds/dsDATA_TYPE_IMPL.h"
#include "../headers/ws/ds/dsDATA_TYPE_CLASS.h"
#include "../headers/ws/ds/dsSTRID.h"
#include "../headers/hcex/haloRAW_INPUT_X360_ENV.h"

// ds_data::dsDATA_TYPE_IMPL<haloRAW_INPUT_X360_ENV>::IsFunc @ 0x823BD618
// Virtual override: forward to the per-type reflection dispatcher.
template<>
int ds_data::dsDATA_TYPE_IMPL<haloRAW_INPUT_X360_ENV>::IsFunc(const dsDATA &inst, dsSTRID id) const
{
    return ds_data::dsDATA_TYPE_CLASS<haloRAW_INPUT_X360_ENV>::IsFunc(inst, id);
}
