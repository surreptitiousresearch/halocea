#include "../headers/ws/ds/dsERROR_HANDLER.h"
#include "../headers/ws/ds/dsTSTRING.h"
#include "../headers/hcex/haloENGINE_CONTROL.h"

// dsERROR_HANDLER<haloENGINE_CONTROL>::GetErrorStr() @ 0x823C5A58
// Reference to the process-wide error-string slot for the haloENGINE_CONTROL error channel: a
// guarded function-local static dsTSTRING, empty-initialized on first use (registered via atexit).
// DEVIATION: the decompiler expands the initialization inline (the UnsafeInitEmpty shared-empty
// singleton adopt + refcount bump); the default dsTSTRING ctor is the source-level equivalent.
template<>
dsTSTRING<char> &dsERROR_HANDLER<haloENGINE_CONTROL>::GetErrorStr()
{
    static dsTSTRING<char> err;
    return err;
}
