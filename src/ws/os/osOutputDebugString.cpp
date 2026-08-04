// osOutputDebugString @0x82612488 — printf-style front-end over _osOutputDebugString.
// Formats the variadic message into a fixed 512-byte buffer (truncating to 511 chars + NUL) and
// emits it to the platform debug output. (The shipped prologue spills the varargs home area before
// va_start, per the PPC calling convention.)
#include <stdio.h>
#include <stdarg.h>

extern void _osOutputDebugString(const char *str); // src/ws/os/_osOutputDebugString.cpp

void osOutputDebugString(const char *fmt, ...)
{
    char buffer[520];

    va_list va;
    va_start(va, fmt);
    vsnprintf(buffer, 0x1FF, fmt, va);
    va_end(va);

    buffer[511] = '\0';
    _osOutputDebugString(buffer);
}
