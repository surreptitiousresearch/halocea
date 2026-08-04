#include "dsTSTRING.h"
#include <stdarg.h>

// Shared per-call scratch string returned by ds's formatted-print helpers, and the vsnprintf-style
// worker that fills it. Both are ds boundaries (deeper callees, not re-sourced here).
extern dsTSTRING<char> *GetDsBufStr();
extern void dsSPrintfVS(dsTSTRING<char> *out, const char *fmt, va_list args);

// dsSPrintf(const char* fmt, ...) @ 0x825210C0
// printf-into-a-dsTSTRING: format into the shared scratch string via dsSPrintfVS, then return a
// copy of it by value (sret) — the result simply shares the scratch buffer and bumps its refcount.
// NOTE: the PPC decompile spells the varargs as a run of homed register slots (a3..a11 + va_arg
// scan); that is the ABI's register-save prologue for the ellipsis, modeled here as a plain
// va_list forward.
dsTSTRING<char> dsSPrintf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    dsSPrintfVS(GetDsBufStr(), fmt, args);
    va_end(args);

    dsTSTRING<char> result;
    dsTSTRING<char> *scratch = GetDsBufStr();
    dsTSTRING_BUF_HEADER<char> *buffer = scratch->pBuffer;
    int refCount = scratch->pBuffer->refCount;
    result.pBuffer = scratch->pBuffer;
    buffer->refCount = refCount + 1;
    return result;
}
