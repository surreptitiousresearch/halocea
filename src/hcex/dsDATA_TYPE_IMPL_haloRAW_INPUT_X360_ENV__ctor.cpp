#include "../headers/ws/ds/dsDATA_TYPE_IMPL.h"
#include "../headers/hcex/haloRAW_INPUT_X360_ENV.h"

// ds_data::dsDATA_TYPE_IMPL<haloRAW_INPUT_X360_ENV>::dsDATA_TYPE_IMPL @ 0x823BCAF8
// Stamp the runtime type id from the global counter and self-register with the ds_data type
// registry. DEVIATION: the two __vftable stores in the disassembly (base dsDATA_TYPE vtable then
// the IMPL specialization vtable) are compiler-emitted ctor prologue, not user-body statements,
// so they are not reproduced here.
template<>
ds_data::dsDATA_TYPE_IMPL<haloRAW_INPUT_X360_ENV>::dsDATA_TYPE_IMPL()
{
    id = ds_data::gTypeId++;
    ds_data::RegisterType(this);
}
