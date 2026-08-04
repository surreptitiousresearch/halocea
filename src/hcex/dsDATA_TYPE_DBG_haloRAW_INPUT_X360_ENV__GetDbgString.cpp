#include "../headers/ws/ds/dsDATA_TYPE_DBG.h"
#include "../headers/ws/ds/dsTSTRING.h"
#include "../headers/hcex/haloRAW_INPUT_X360_ENV.h"

// ds_data::dsDATA_TYPE_DBG<haloRAW_INPUT_X360_ENV>::GetDbgString(const haloRAW_INPUT_X360_ENV&) @ 0x823C05B0
// No bespoke formatter for this type: yield the literal "undef".
template<>
dsTSTRING<char> ds_data::dsDATA_TYPE_DBG<haloRAW_INPUT_X360_ENV>::GetDbgString(const haloRAW_INPUT_X360_ENV &val)
{
    (void)val;
    dsTSTRING<char> result;
    result.pBuffer = nullptr;
    result.UnsafeInit("undef", -1, 0);
    return result;
}
