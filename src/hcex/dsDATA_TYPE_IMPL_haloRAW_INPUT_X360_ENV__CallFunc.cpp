#include "../headers/ws/ds/dsDATA_TYPE_IMPL.h"
#include "../headers/ws/ds/dsDATA_TYPE_CLASS.h"
#include "../headers/ws/ds/dsSTRID.h"
#include "../headers/hcex/haloRAW_INPUT_X360_ENV.h"

// ds_data::dsDATA_TYPE_IMPL<haloRAW_INPUT_X360_ENV>::CallFunc @ 0x823BD688
// Virtual override: forward to the per-type reflection dispatcher.
template<>
int ds_data::dsDATA_TYPE_IMPL<haloRAW_INPUT_X360_ENV>::CallFunc(
        dsSTRID id, dsDATA &inst, dsDATA *args, int argCount, dsDATA &retval) const
{
    return ds_data::dsDATA_TYPE_CLASS<haloRAW_INPUT_X360_ENV>::CallFunc(id, inst, args, argCount, retval);
}
