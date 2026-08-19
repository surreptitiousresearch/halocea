/* usprintf @0x8376D840 — variadic wchar_t sprintf wrapper; forwards straight to _vswprintf.
 * Attestation: 0/12 sampled callers (of 21) consume r3 (callee residue) — declared return is void. */

#include <stddef.h>   /* wchar_t */
#include <stdarg.h>

extern int _vswprintf(wchar_t *string, const wchar_t *format, va_list args);

void usprintf(wchar_t *string, const wchar_t *format, ...)
{
    va_list args;
    va_start(args, format);
    _vswprintf(string, format, args);
    va_end(args);
}
