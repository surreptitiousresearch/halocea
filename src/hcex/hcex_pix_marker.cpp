/* hcex_pix_marker @0x823CEC88 — format a message and drop a PIX profiler marker, but only when PIX
 * instrumentation is enabled. vsnprintf into a 128-byte stack buffer (clamped to 127 chars + NUL) then
 * osPIXAddMarker.
 *
 * Deviation: the decompiler mis-modeled the PowerPC varargs prologue (r4..r10 register save-area stores) as
 * fixed 64-bit params with the format string packed into HIDWORD(text); the true signature is
 * (const char *fmt, ...). osPIXAddMarker is a ws-engine profiling boundary. */

#include <stdarg.h>

extern unsigned char hcex_enable_pix;   /* bool hcex_enable_pix */
extern int vsnprintf(char *buffer, unsigned int count, const char *format, va_list args);
extern void osPIXAddMarker(const char *string);

extern "C" void hcex_pix_marker(const char *fmt, ...)
{
    char buffer[128];
    va_list args;

    va_start(args, fmt);
    if ( hcex_enable_pix )
    {
        vsnprintf(buffer, 0x7F, fmt, args);
        buffer[127] = 0;
        osPIXAddMarker(buffer);
    }
    va_end(args);
}
