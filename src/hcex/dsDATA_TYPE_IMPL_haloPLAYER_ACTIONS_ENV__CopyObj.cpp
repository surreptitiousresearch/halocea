#include "../headers/ws/ds/dsDATA_TYPE_IMPL.h"
#include "../headers/ws/ds/dsDATA_TYPE_CONSTRUCT.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"

// ds_data::dsDATA_TYPE_IMPL<haloPLAYER_ACTIONS_ENV>::CopyObj @ 0x823BEB80
// Virtual override: overwrite the boxed value at the slot from a raw object via the construct policy.
template<>
void ds_data::dsDATA_TYPE_IMPL<haloPLAYER_ACTIONS_ENV>::CopyObj(const void *obj, unsigned int *dest) const
{
    ds_data::dsDATA_TYPE_CONSTRUCT<haloPLAYER_ACTIONS_ENV>::CopyObj(
        static_cast<const haloPLAYER_ACTIONS_ENV *>(obj), dest);
}
