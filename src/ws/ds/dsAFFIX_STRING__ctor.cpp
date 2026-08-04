#include "dsAFFIX_STRING.h"

// dsAFFIX_STRING::dsAFFIX_STRING() @ 0x823B1458
// CAVEAT: `str` default-member-inits to the shared empty-string singleton (dsTSTRING<char>'s own
// default ctor), then the body below re-adopts a SECOND empty temporary and assigns it over `str`
// via operator=, destroying the temporary afterward. That double-init reads as redundant, but it is
// exactly what the decompile shows (two independent inlined UnsafeInitEmpty sequences plus a
// self-checked release/adopt/refcount-bump) -- reproduced faithfully rather than "optimized away".
dsAFFIX_STRING::dsAFFIX_STRING()
{
    str = dsTSTRING<char>();
}
