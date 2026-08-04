#include "../headers/ws/ds/dsDATA_TYPE_IMPL.h"
#include "../headers/ws/ds/dsDATA_TYPE_CONSTRUCT.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"

// ds_data::dsDATA_TYPE_IMPL<haloPLAYER_ACTIONS_ENV>::Destroy @ 0x823BEB18
// Virtual override: free the boxed value via the construct policy's Destroy.
template<>
void ds_data::dsDATA_TYPE_IMPL<haloPLAYER_ACTIONS_ENV>::Destroy(unsigned int *storage) const
{
    ds_data::dsDATA_TYPE_CONSTRUCT<haloPLAYER_ACTIONS_ENV>::Destroy(storage);
}
