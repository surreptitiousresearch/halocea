/* csprintf @0x836F5B40 */
#include <stdarg.h>

extern int vsprintf_0(char *string, const char *format, va_list ap);

/* Formats into `buffer` and returns it. Thin varargs wrapper over vsprintf_0.
 * DEVIATION: the decompiler exposes the PPC varargs register-homing (a3..a11 spills)
 * as explicit params; the source form is a plain ... forwarded via va_list. */
char * csprintf(char *buffer, const char *format, ...)
{
    va_list va;
    va_start(va, format);
    vsprintf_0(buffer, format, va);
    va_end(va);
    return buffer;
}
