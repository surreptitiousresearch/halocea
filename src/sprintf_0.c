/* sprintf_0 @ 0x838867E0 — CRT sprintf clone.
 * Varargs proven in disasm: prologue spills r5..r10 to the caller home area
 * (0x838867E8..FC) and passes the argptr to _output_l @ 0x83886888.
 * DEVIATION (portable paraphrase): the binary inlines the CRT internals — builds a
 * 0x1C-byte fake FILE on stack (_ptr/_base=string, _cnt=0x7FFFFFFF, _flag=0x42
 * = _IOWRT|_IOSTRG), calls _output_l, then NUL-terminates (or _flsbuf on overflow);
 * on NULL format/string it sets *_errno()=22 and calls _invalid_parameter_noinfo()
 * before returning -1. Modeled here via vsprintf/errno; same observable behavior
 * minus the invalid-parameter hook. */

#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

int sprintf_0(char *string, const char *format, ...)
{
    va_list args;
    int     result;

    if (!format || !string)
    {
        errno = 22;
        return -1;
    }
    va_start(args, format);
    result = vsprintf(string, format, args);
    va_end(args);
    return result;
}
