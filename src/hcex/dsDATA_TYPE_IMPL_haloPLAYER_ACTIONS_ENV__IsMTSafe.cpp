#include "../headers/ws/ds/dsDATA_TYPE_IMPL.h"
#include "../headers/ws/ds/dsDATA_TYPE_CONSTRUCT.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"

// ds_data::dsDATA_TYPE_IMPL<haloPLAYER_ACTIONS_ENV>::IsMTSafe @ 0x823BCAB8
// Virtual override: this heap-boxed type is never MT-safe. The GetPtr probe is retained from the
// reversed body (its result is discarded); the method always reports 0.
template<>
int ds_data::dsDATA_TYPE_IMPL<haloPLAYER_ACTIONS_ENV>::IsMTSafe(const unsigned int *storage) const
{
    ds_data::dsDATA_TYPE_CONSTRUCT<haloPLAYER_ACTIONS_ENV>::GetPtr(storage);
    return 0;
}
