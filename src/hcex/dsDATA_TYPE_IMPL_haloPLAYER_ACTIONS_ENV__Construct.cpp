#include "../headers/ws/ds/dsDATA_TYPE_IMPL.h"
#include "../headers/ws/ds/ds_boundary.h"
#include "../headers/hcex/halo_ds_data_type_boundary.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"

// ds_data::dsDATA_TYPE_IMPL<haloPLAYER_ACTIONS_ENV>::Construct @ 0x823BFBE8
// Virtual override: construct a fresh slot value from the type's function-static default instance.
// The local-static guard + defInst the decompiler surfaces here is the inlined body of
// dsDATA_DEFAULT_VALUE<haloPLAYER_ACTIONS_ENV>::GetDefaultVal (the only init it performs is
// zeroing skip_cine).
template<>
void ds_data::dsDATA_TYPE_IMPL<haloPLAYER_ACTIONS_ENV>::Construct(unsigned int *storage) const
{
    ds_data::dsDATA_TYPE_CONSTRUCT<haloPLAYER_ACTIONS_ENV>::Construct(
        storage, &ds_data::dsDATA_DEFAULT_VALUE<haloPLAYER_ACTIONS_ENV>::GetDefaultVal());
}
