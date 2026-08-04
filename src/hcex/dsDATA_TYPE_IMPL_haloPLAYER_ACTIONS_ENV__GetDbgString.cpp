#include "../headers/ws/ds/dsDATA_TYPE_IMPL.h"
#include "../headers/ws/ds/dsDATA_TYPE_CONSTRUCT.h"
#include "../headers/ws/ds/dsTSTRING.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"

// ds_data::dsDATA_TYPE_IMPL<haloPLAYER_ACTIONS_ENV>::GetDbgString(const unsigned int*) const @ 0x823C0648
// Virtual debug-format override: touches the boxed value then yields the literal "undef".
// DEVIATION: the PPC decompiler labels r3 (the hidden struct-return buffer) as `this` and r4 (the
// real IMPL this) as `result`; the body actually writes into the sret buffer and leaves the IMPL
// object unused. Modeled here as the by-value return the mangle (?A?AV?$dsTSTRING@D@@...) specifies.
template<>
dsTSTRING<char> ds_data::dsDATA_TYPE_IMPL<haloPLAYER_ACTIONS_ENV>::GetDbgString(const unsigned int *storage) const
{
    ds_data::dsDATA_TYPE_CONSTRUCT<haloPLAYER_ACTIONS_ENV>::GetPtr(storage); // box access; result discarded
    dsTSTRING<char> result;
    result.pBuffer = nullptr;
    result.UnsafeInit("undef", -1, 0);
    return result;
}
