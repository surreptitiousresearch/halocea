#include "../headers/ws/ds/dsDATA_TYPE_IMPL.h"
#include "../headers/ws/ds/dsDATA_TYPE_HELPERS.h"
#include "../headers/ws/ds/dsSTRID.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"

// ds_data::dsDATA_TYPE_IMPL<haloPLAYER_ACTIONS_ENV>::CallFunc() @ 0x823BC9D8
// Virtual override forwarding to the static-policy twin.
template<>
int ds_data::dsDATA_TYPE_IMPL<haloPLAYER_ACTIONS_ENV>::CallFunc(
    dsSTRID id, dsDATA &inst, dsDATA *args, int argCount, dsDATA &retval) const
{
    return ds_data::dsDATA_TYPE_CLASS<haloPLAYER_ACTIONS_ENV>::CallFunc(id, inst, args, argCount, retval);
}
