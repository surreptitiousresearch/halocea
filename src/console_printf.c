/* console_printf @0x83721F38 — formatted print to the in-game terminal: optionally clear the terminal first,
 * format the message (truncated to 255 chars), print it, and, when console logging is enabled, append it to
 * the error file.
 *
 * Deviation: the decompiler's manual va_arg register-shuffling is PowerPC varargs ABI noise; reproduced with a
 * standard va_list. */

#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include "headers/blam_data_globals.h"

extern void terminal_clear(void);
extern int vsprintf_0(char *buffer, const char *format, va_list args);
extern void terminal_printf(const real_argb_color *color, const char *format, ...);
extern void write_to_error_file(char *string, uint8_t date);

void console_printf(uint8_t clear, const char *format, ...)
{
    char buffer[1032];
    va_list args;

    if ( clear )
        terminal_clear();

    va_start(args, format);
    vsprintf_0(buffer, format, args);
    va_end(args);
    buffer[255] = 0;

    terminal_printf(0, "%s", buffer);
    if ( console_dump_to_file )
    {
        strncat(buffer, "\r\n", 0x400u);
        write_to_error_file(buffer, 1u);
    }
}
