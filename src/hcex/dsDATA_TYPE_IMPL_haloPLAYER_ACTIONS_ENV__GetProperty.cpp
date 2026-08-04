#include "../headers/ws/ds/dsDATA_TYPE_IMPL.h"
#include "../headers/ws/ds/dsDATA_TYPE_CLASS.h"
#include "../headers/ws/ds/dsSTRID.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"

// ds_data::dsDATA_TYPE_IMPL<haloPLAYER_ACTIONS_ENV>::GetProperty @ 0x823BCA28
// Virtual override: forward to the per-type reflection dispatcher.
template<>
int ds_data::dsDATA_TYPE_IMPL<haloPLAYER_ACTIONS_ENV>::GetProperty(
        dsSTRID id, const dsDATA &inst, dsDATA &val) const
{
    return ds_data::dsDATA_TYPE_CLASS<haloPLAYER_ACTIONS_ENV>::GetProperty(id, inst, val);
}
