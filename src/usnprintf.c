#include <wchar.h>
#include <stdarg.h>

/* usnprintf — bounded formatted print into a UTF-16 buffer. Sets up a va_list over the variadic
 * arguments and forwards to the CRT wide vsnprintf. The disassembly spills the variadic register
 * home area (r6..r10) into the frame and calls `_vsnwprintf(string, size, format, va)`.
 * Attestation: 0/12 sampled callers (of 62) consume r3 (callee residue) — declared return is void. */
/* _vsnwprintf provided by CRT via <wchar.h>; local extern removed (C28251: the local redeclaration drops the header's annotations) */

void usnprintf(wchar_t *string, unsigned int size, const wchar_t *format, ...)
{
    va_list args;

    va_start(args, format);
    _vsnwprintf(string, size, format, args);
    va_end(args);
}
