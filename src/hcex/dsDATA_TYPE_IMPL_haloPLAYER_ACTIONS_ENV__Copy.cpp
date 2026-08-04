#include "../headers/ws/ds/dsDATA_TYPE_IMPL.h"
#include "../headers/ws/ds/dsDATA_TYPE_CONSTRUCT.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"

// ds_data::dsDATA_TYPE_IMPL<haloPLAYER_ACTIONS_ENV>::Copy @ 0x823BEB48
// Virtual override: copy an already-constructed boxed value between slots via the construct policy.
template<>
void ds_data::dsDATA_TYPE_IMPL<haloPLAYER_ACTIONS_ENV>::Copy(const unsigned int *src, unsigned int *dest) const
{
    ds_data::dsDATA_TYPE_CONSTRUCT<haloPLAYER_ACTIONS_ENV>::Copy(src, dest);
}
