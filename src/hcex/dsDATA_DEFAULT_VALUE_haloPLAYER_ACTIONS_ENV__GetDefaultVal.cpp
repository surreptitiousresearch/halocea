#include "../headers/ws/ds/ds_boundary.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"

// ds_data::dsDATA_DEFAULT_VALUE<haloPLAYER_ACTIONS_ENV>::GetDefaultVal() @ 0x823BBBE8
// Return a reference to a lazily-initialised function-static default instance. The compiler's
// local-static guard is the "local static guard" the decompiler surfaces; the only init the
// inlined default-ctor performs is zeroing skip_cine (u0/u1 stay default — see the type header).
template<>
const haloPLAYER_ACTIONS_ENV &ds_data::dsDATA_DEFAULT_VALUE<haloPLAYER_ACTIONS_ENV>::GetDefaultVal()
{
    static haloPLAYER_ACTIONS_ENV defInst;
    return defInst;
}
