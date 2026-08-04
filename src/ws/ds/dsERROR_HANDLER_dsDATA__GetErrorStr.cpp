#include "dsERROR_HANDLER.h"
#include "dsDATA.h"

// dsERROR_HANDLER<dsDATA>::GetErrorStr @ 0x825AB1A8
// Static accessor returning (by value, via sret) the current error message for the dsDATA error
// channel. Backed by a function-local "empty error string" singleton that shares the process-wide
// empty-string buffer; lazily initialised on first call and released via atexit.
template<>
dsTSTRING<char> &dsERROR_HANDLER<dsDATA>::GetErrorStr() // returns ref to the static (matches decl)
{
    static dsTSTRING<char> err;
    static bool errInited = false; // compiler-emitted one-time local-static guard
    if (!errInited) {
        errInited = true;
        err.pBuffer = nullptr;
        // dsTSTRING<char>::UnsafeInitEmpty (inlined in the binary): point `err` at the shared,
        // lazily-allocated empty-string buffer and take a reference on it.
        err.UnsafeInitEmpty();
    }
    return err; // sret copy: the returned string shares `err`s buffer (its refcount is bumped)
}
