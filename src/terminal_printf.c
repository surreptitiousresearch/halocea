/* terminal_printf @0x836A3890 — append a formatted, colored line to the in-game terminal output (only in
 * developer mode >= 4, when the terminal is initialized). A null color defaults to a light-grey (1, .7, .7,
 * .7) ARGB. The "|t" marker in the formatted text sets the line's tab flag.
 *
 * Deviation: the PowerPC varargs register-shuffling is ABI noise; reproduced with a standard va_list. */

#include "headers/developer_mode.h"
#include "headers/real_argb_color.h"
#include "headers/terminal_globals.h"
#include "headers/output_line_datum.h"
#include "headers/error_global_data.h"
#include <stdarg.h>
#include "headers/blam_data_globals.h"

extern int terminal_new_line(void);
/* vsnprintf provided by CRT via <stdio.h>; local extern removed (C28251: the local redeclaration drops the header's annotations) */
extern char *strstr(const char *haystack, const char *needle);

void terminal_printf(const real_argb_color *color, const char *format, ...)
{
    real_argb_color default_color;
    default_color.n[0] = 1.0f;
    default_color.n[1] = 0.69999999f;
    default_color.n[2] = 0.69999999f;
    default_color.n[3] = 0.69999999f;

    if ( error_globals.developer_mode >= _developer_mode_conprint_enabled && terminal_globals.initialized )
    {
        short line_index = terminal_new_line();
        if ( line_index != -1 )
        {
            output_line_datum *line =
                &((output_line_datum *)terminal_globals.output_lines->data)[(unsigned short)line_index];
            const real_argb_color *line_color = color ? color : &default_color;

            line->timer = 0;   /* reset age counter for the new line */
            line->color = *line_color;

            va_list args;
            va_start(args, format);
            vsnprintf(line->buffer, 0xFEu, format, args);
            va_end(args);
            line->tabstop = strstr(line->buffer, "|t") != 0;
        }
    }
}
