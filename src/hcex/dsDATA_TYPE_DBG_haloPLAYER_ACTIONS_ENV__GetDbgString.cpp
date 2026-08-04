#include "../headers/ws/ds/dsDATA_TYPE_DBG.h"
#include "../headers/ws/ds/dsTSTRING.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"

// ds_data::dsDATA_TYPE_DBG<haloPLAYER_ACTIONS_ENV>::GetDbgString(const haloPLAYER_ACTIONS_ENV&) @ 0x823C0560
// No bespoke formatter for this type: yield the literal "undef".
template<>
dsTSTRING<char> ds_data::dsDATA_TYPE_DBG<haloPLAYER_ACTIONS_ENV>::GetDbgString(const haloPLAYER_ACTIONS_ENV &val)
{
    (void)val;
    dsTSTRING<char> result;
    result.pBuffer = nullptr;
    result.UnsafeInit("undef", -1, 0);
    return result;
}
