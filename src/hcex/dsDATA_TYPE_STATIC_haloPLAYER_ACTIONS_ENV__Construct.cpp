#include "../headers/ws/ds/ds_boundary.h"
#include "../headers/hcex/halo_ds_data_type_boundary.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"

// ds_data::dsDATA_TYPE_STATIC<haloPLAYER_ACTIONS_ENV>::Construct @ 0x823BF638
// Default-construct the inline storage slot from the type's default value.
// DEVIATION: the disassembly inlines dsDATA_DEFAULT_VALUE<T>::GetDefaultVal() as a function-local
// static (guarded init that zeroes defInst.skip_cine); the source form is a call to GetDefaultVal,
// reproduced here as such (GetDefaultVal remains a boundary declaration).
template<>
void ds_data::dsDATA_TYPE_STATIC<haloPLAYER_ACTIONS_ENV>::Construct(unsigned int *storage)
{
    ds_data::dsDATA_TYPE_CONSTRUCT<haloPLAYER_ACTIONS_ENV>::Construct(
        storage, &ds_data::dsDATA_DEFAULT_VALUE<haloPLAYER_ACTIONS_ENV>::GetDefaultVal());
}
