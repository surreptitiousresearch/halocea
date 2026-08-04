#include "dsTSTRING.h"

// dsTSTRING<char>::Clear @ 0x823D5A38
// Reset the string to empty and return *this. If the backing buffer is uniquely owned
// (refCount == 1), truncate it in place; otherwise drop our reference and adopt the process-wide
// shared empty-string singleton.
//
// CAVEAT: the shared-buffer branch was inlined by the compiler. The disasm expands
// dsTSTRING<char>::UnsafeInitEmpty (lazy singleton allocation + refCount bump) and an additional
// bump/release pair around the singleton (the teardown of a temporary the source built for the
// assignment). The net reference-count effect on the singleton is +1 — identical to a single
// UnsafeInitEmpty — so it is modeled here as ReleaseBuffer(old) + UnsafeInitEmpty().
template<>
dsTSTRING<char> &dsTSTRING<char>::Clear()
{
    dsTSTRING_BUF_HEADER<char> *buffer = this->pBuffer;
    if (buffer->refCount == 1) {
        // Uniquely owned: truncate in place.
        buffer->strLen = 0;
        this->pBuffer->str[this->pBuffer->strLen] = 0;
    } else {
        // Shared: drop one reference (refCount > 1, so it never frees here) and adopt the
        // shared empty-string singleton.
        this->ReleaseBuffer(buffer);
        this->UnsafeInitEmpty();
    }
    return *this;
}
