#include "wb_boundary.h"

namespace wb {

// wb::RegisterPage(const char*, bool, int) @ 0x8269E5F8 -- register a new watch page: appends
// an empty text buffer to `pages`, a dbgVARWrapper("is this page active") to `vars`, and the
// caller's flags to `pageFlags`; returns the new page's index (pages.nElem - 1).
//
// Deviation from the raw decompile: the pseudocode shows the guarded-static "emptyStr"
// lazy-init (UnsafeInitEmpty's local-static singleton) inlined by hand, and constructs
// dbgVARWrapper's fields (dsSTRID ctor + raw stores) directly rather than calling the
// dbgVARWrapper(const char*, bool) constructor symbol. Disasm confirms this is exactly that
// constructor's body inlined at the call site (0x8269e6ac-0x8269e6bc), so it is reconstructed
// here as a plain dbgVARWrapper construction; the emptyStr singleton is left as dsTSTRING<char>
// default-construction (an empty string), which is what UnsafeInitEmpty ultimately produces.
int RegisterPage(const char *dbgVarName, bool dbgVarDefValue, int flags)
{
    dsTSTRING<char> emptyPage; // adopts the shared empty-string singleton, per UnsafeInitEmpty
    wb::pages.PushBack(emptyPage);

    wb::dbgVARWrapper varWrapper(dbgVarName, dbgVarDefValue);
    wb::vars.PushBack(varWrapper);
    // varWrapper's destructor runs here (scope exit): its dbgVar is still null (PushBack copy-
    // constructs vars' new slot and never mutates varWrapper), so this is a no-op in practice --
    // matches the disasm's dead-looking `if (dbgVar) { vtbl-dtor; dbgVar = nullptr; }` at
    // 0x8269e6d0-0x8269e6ec, which is dbgVARWrapper::~dbgVARWrapper inlined at scope exit.

    wb::pageFlags.PushBack(flags);

    return wb::pages.nElem - 1;
}

} // namespace wb
