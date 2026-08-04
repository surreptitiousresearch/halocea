/* ufprintf @0x8376D758 — variadic wchar_t fprintf wrapper; forwards straight to vfwprintf.
   (Decompiler mis-rendered the PPC varargs register homing as fixed a3..a11 params;
   the canonical source is a simple va_start/vfwprintf/va_end forwarder.) */

#include <stdarg.h>
#include <stdio.h>

int ufprintf(FILE *stream, const wchar_t *format, ...)
{
    va_list args;
    va_start(args, format);
    int result = vfwprintf(stream, format, args);
    va_end(args);
    return result;
}
