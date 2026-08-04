#include "../headers/ws/ds/dsDATA_TYPE_IMPL.h"
#include "../headers/ws/ds/dsDATA_TYPE_CONSTRUCT.h"
#include "../headers/ws/ds/dsTSTRING.h"
#include "../headers/hcex/haloRAW_INPUT_X360_ENV.h"

// ds_data::dsDATA_TYPE_IMPL<haloRAW_INPUT_X360_ENV>::GetDbgString(const unsigned int*) const @ 0x823C06A8
// Virtual debug-format override: touches the boxed value then yields the literal "undef".
// DEVIATION: the PPC decompiler labels r3 (the hidden struct-return buffer) as `this` and r4 (the
// real IMPL this) as `result`; the body actually writes into the sret buffer and leaves the IMPL
// object unused. Modeled here as the by-value return the mangle specifies.
template<>
dsTSTRING<char> ds_data::dsDATA_TYPE_IMPL<haloRAW_INPUT_X360_ENV>::GetDbgString(const unsigned int *storage) const
{
    ds_data::dsDATA_TYPE_CONSTRUCT<haloRAW_INPUT_X360_ENV>::GetPtr(storage); // box access; result discarded
    dsTSTRING<char> result;
    result.pBuffer = nullptr;
    result.UnsafeInit("undef", -1, 0);
    return result;
}
