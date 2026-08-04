#include "dsDATA_SIMPLE_WRAPPER.h"
#include "dsTSTRING.h"

// dsDATA_SIMPLE_WRAPPER<dsTSTRING<char>>::dsDATA_SIMPLE_WRAPPER(const dsDATA&, int*) @ 0x82504528
// Retrieve a dsTSTRING<char> out of `data` (DSD_CONV_RETRIEVE) into `val`. On failure, if `isOk`
// is non-null and still true (nonzero), clear it to record the failure.
// CAVEAT: the decompiler's prologue shows `val` being explicitly set to the shared empty-string
// singleton (a lazily-initialised static `emptyStr`, guarded and atexit-registered) before the
// GetValue call -- that sequence is exactly dsTSTRING<char>'s own default constructor
// (UnsafeInitEmpty) inlined at this constructor's member-init point. Reconstructed here as an
// implicit default member-init of `val` (dsTSTRING<char>'s default ctor already does this),
// followed only by the explicit retrieval logic.
template<>
dsDATA_SIMPLE_WRAPPER<dsTSTRING<char>>::dsDATA_SIMPLE_WRAPPER(const dsDATA &data, int *isOk)
{
    if (!data.GetValue(val, DSD_CONV_RETRIEVE) && isOk) {
        if (*isOk)
            *isOk = 0;
    }
}
