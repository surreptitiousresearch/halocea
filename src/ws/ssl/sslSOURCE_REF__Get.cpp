#include "sslSOURCE_REF.h"
#include "sslSOURCE.h"
#include "../ds/dsTSTRING.h"

// 0x825CB9C8 — sslSOURCE_REF::Get. Copy of the referenced source's text, or the shared empty string
// when this ref is empty. Returns dsTSTRING<char> by value (sret): the decompiler swaps this/result
// (r3 = return slot dsTSTRING*, r4 = this sslSOURCE_REF*).
//
// CAVEAT: the decompiler expands this into a two-temporary accumulator with a branch-selected flag
// (one for the source-text copy, two for the empty singleton) and manual refcount fixups. That is
// the compiler's by-value/sret bookkeeping; the reconstruction below returns by value directly.
dsTSTRING<char> sslSOURCE_REF::Get() const
{
    if (this->source)
        return this->source->Get();

    dsTSTRING<char> empty;
    empty.UnsafeInitEmpty(); // shared empty-string singleton
    return empty;
}
