#include "../headers/ws/ds/dsDATA_TYPE_IMPL.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"

// ds_data::dsDATA_TYPE_IMPL<haloPLAYER_ACTIONS_ENV>::dsDATA_TYPE_IMPL() @ 0x823BBAE0
// Stamp the runtime type id from the global monotonic counter, then self-register with the
// ds_data type registry. The two vtable stores the decompiler shows (base dsDATA_TYPE, then the
// IMPL override table) are the implicit ctor vtable installs — emitted automatically here.
template<>
ds_data::dsDATA_TYPE_IMPL<haloPLAYER_ACTIONS_ENV>::dsDATA_TYPE_IMPL()
{
    this->id = ds_data::gTypeId++;
    ds_data::RegisterType(this);
}
