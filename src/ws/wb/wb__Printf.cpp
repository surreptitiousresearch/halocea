#include <cstdarg>
#include "wb_boundary.h"

// dsSPrintfV(format, va_list) @ ?dsSPrintfV@@YA?AV?$dsTSTRING@D@@PBDPAD@Z -- global-namespace
// helper (not in wb/ds) that formats `format` against an already-open varargs cursor and
// returns the result by value (sret). This binary's va_list is a plain pointer into the
// spilled register-argument area (Xbox 360 PPC ABI), matching the `char*` 2nd parameter here.
extern dsTSTRING<char> dsSPrintfV(const char *format, char *args); // boundary — C++ linkage per DB mangling ?dsSPrintfV@@YA?AV?$dsTSTRING@D@@PBDPAD@Z
extern "C" void *memcpy(void *dst, const void *src, unsigned int n);  // boundary — CRT
extern "C" void dlFree(void *ptr);                                    // boundary — dlmalloc free

namespace wb {

// wb::Printf(int, const char*, ...) @ 0x8269E238 -- append a formatted line to page `page`'s
// accumulated text buffer.
//
// CAVEAT: the decompiler's pseudocode is badly mangled here ("local variable allocation has
// failed" -- a dozen fabricated __int64 scratch params and manual va_copy/va_arg churn). Disasm
// (0x8269e238-0x8269e2e8) shows the real body is much simpler: it opens one va_list over the
// spilled varargs, calls dsSPrintfV once to format them into a temporary dsTSTRING, then
// splices that temporary's text onto the end of pages[page] via dsTSTRING::Insert(pos,len) +
// memcpy, and finally releases the temporary's buffer reference. Reconstructed from disasm,
// not the decompiler's pseudocode.
void Printf(int page, const char *str, ...)
{
    va_list args;
    va_start(args, str);
    dsTSTRING<char> formatted = dsSPrintfV(str, (char *)args);
    va_end(args);

    dsTSTRING<char> *pageText = wb::GetPage(page);
    int insertPos = pageText->pBuffer->strLen;
    int insertLen = formatted.pBuffer->strLen;

    // The original calls the protected gap-opener Insert(int,int) (disasm 0x8269e29c). To avoid
    // widening the shared dsTSTRING.h API for this single caller, use the public
    // Insert(pos, char, count) overload to open an insertLen-char gap — its fill char is
    // immediately overwritten by the memcpy below, so the result is identical.
    pageText->Insert(insertPos, (char)0, insertLen);

    // Re-fetch pageText->pBuffer after Insert (it may have reallocated) before writing the
    // newly opened gap -- matches the disassembly's reload at 0x8269e2a8.
    memcpy(&pageText->pBuffer->str[insertPos], formatted.pBuffer->str, insertLen);

    dsTSTRING_BUF_HEADER<char> *formattedBuffer = formatted.pBuffer;
    if (--formattedBuffer->refCount == 0)
        dlFree(formattedBuffer);
}

} // namespace wb
