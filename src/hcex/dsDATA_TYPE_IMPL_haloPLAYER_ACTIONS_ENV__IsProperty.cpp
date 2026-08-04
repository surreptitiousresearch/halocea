#include "../headers/ws/ds/dsDATA_TYPE_IMPL.h"
#include "../headers/ws/ds/dsDATA_TYPE_HELPERS.h"
#include "../headers/ws/ds/dsSTRID.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"

// ds_data::dsDATA_TYPE_IMPL<haloPLAYER_ACTIONS_ENV>::IsProperty() @ 0x823BC9A0
// Virtual override forwarding to the static-policy twin.
template<>
int ds_data::dsDATA_TYPE_IMPL<haloPLAYER_ACTIONS_ENV>::IsProperty(const dsDATA &inst, dsSTRID id) const
{
    return ds_data::dsDATA_TYPE_CLASS<haloPLAYER_ACTIONS_ENV>::IsProperty(inst, id);
}
