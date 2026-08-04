/* console_printf_only_if_open @ 0x83721FE8 — printf to the console, but only while it is open.
 * Optionally clears the terminal first, and mirrors the line to the error file when console
 * dumping is enabled. */

#include <stdint.h>
#include <stdarg.h>
#include "headers/console_globals.h"
#include "headers/blam_data_globals.h"

extern void terminal_clear(void);
extern int  vsprintf_0(char *buffer, const char *format, va_list args);
extern void ship_terminal_printf(const real_argb_color *color, const char *format, ...);
extern char *strncat(char *dst, const char *src, unsigned int n);
extern void write_to_error_file(char *string, uint8_t date);


void console_printf_only_if_open(uint8_t clear, const char *format, ...)
{
    char line[1032];
    va_list args;

    if ( !console_globals.active )
        return;

    if ( clear )
        terminal_clear();

    va_start(args, format);
    vsprintf_0(line, format, args);
    va_end(args);
    line[255] = 0;

    ship_terminal_printf(0, "%s", line);

    if ( console_dump_to_file )
    {
        strncat(line, "\r\n", 0x400u);
        write_to_error_file(line, 1u);
    }
}
