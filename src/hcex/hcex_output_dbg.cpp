/* hcex_output_dbg @0x823CE2F8 — format a debug message and emit it to the platform debug console.
 * vsnprintf into a 512-byte stack buffer (clamped to 511 chars + NUL) then _osOutputDebugString.
 *
 * Deviation: the decompiler mis-modeled the PowerPC varargs prologue (the r4..r10 register save-area
 * stores) as a run of fixed 64-bit params with the format string packed into HIDWORD(text); the disasm
 * (r3 = "format", r4..r10 spilled to the save area) confirms the true signature is (const char *fmt, ...).
 * _osOutputDebugString is a ws-engine debug boundary. */

#include <stdarg.h>

extern "C" int vsnprintf(char *buffer, unsigned int count, const char *format, va_list args);
extern void _osOutputDebugString(const char *string);

extern "C" void hcex_output_dbg(const char *fmt, ...)
{
    char buffer[512];
    va_list args;

    va_start(args, fmt);
    vsnprintf(buffer, 0x1FF, fmt, args);
    va_end(args);
    buffer[511] = 0;
    _osOutputDebugString(buffer);
}
