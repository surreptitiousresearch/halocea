#include "dsTSTRING.h"

// dsTSTRING<char>::SubStr @ 0x824FADD0
// Return the substring of `len` characters starting at `pos` (len==-1 means "to end of string"), by
// value. A zero-length result adopts the process-wide shared empty-string singleton.
//
// CAVEAT: sret/this ABI swap. The decompiler labels the hidden return-value pointer as `this` and the
// real (const) source string as `result`; reconstructed here with the true C++ signature — `this` is
// the const source and `out` is the returned string. The empty-string else-branch is the inlined body
// of UnsafeInitEmpty (function-static lazily-allocated singleton) and is modeled as that call.
template<>
dsTSTRING<char> dsTSTRING<char>::SubStr(int pos, int len) const
{
    dsTSTRING<char> out;

    int subLen = len;
    if (len == -1)
        subLen = this->pBuffer->strLen - pos;

    if (subLen) {
        const char *src = &this->pBuffer->str[pos];
        out.pBuffer = nullptr;
        out.UnsafeInit(src, subLen, 0);
    } else {
        out.pBuffer = nullptr;
        out.UnsafeInitEmpty();
    }
    return out;
}
