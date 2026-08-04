/* uprintf @0x8376D7A8 — formatted print of a UTF-16 string to stdout. Sets up a va_list over the
 * variadic arguments and forwards to the CRT wide vwprintf. */

#include <wchar.h>
#include <stdarg.h>

int uprintf(const wchar_t *format, ...)
{
    va_list args;
    int result;

    va_start(args, format);
    result = vwprintf(format, args);
    va_end(args);

    return result;
}
