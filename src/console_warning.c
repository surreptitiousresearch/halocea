/* console_warning @0x837220A8 — format a warning message, print it to the in-game terminal in red, and
 * (when console_dump_to_file is set) append it to the error log file. The formatted text is truncated to
 * 255 characters before display.
 *
 * DEVIATION: the decompiler's register allocation failed here (variadic + PPC soft-float ABI), producing a
 * garbled prototype and HIDWORD(format) artifacts; this is the straightforward reconstruction. */

#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include "headers/real_argb_color.h"
#include "headers/blam_data_globals.h"


extern int vsprintf_0(char *buffer, const char *format, va_list args);
extern void terminal_printf(const real_argb_color *color, const char *format, ...);
extern void write_to_error_file(char *string, uint8_t date);

void console_warning(const char *format, ...)
{
    char buffer[1032];

    va_list args;
    va_start(args, format);
    vsprintf_0(buffer, format, args);
    va_end(args);

    buffer[255] = 0;
    terminal_printf(global_real_argb_red, "%s", buffer);

    if ( console_dump_to_file )
    {
        strncat(buffer, "\r\n", 0x400u);
        write_to_error_file(buffer, 1u);
    }
}
