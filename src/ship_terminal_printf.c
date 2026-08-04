/* ship_terminal_printf @0x836A39C0 — append a formatted, colored line to the console/terminal output ring.
 * If no color is supplied a default gray (argb 1.0, 0.7, 0.7, 0.7) is used. Does nothing until the terminal
 * subsystem is initialized. The text is formatted with vsnprintf into the line record; a trailing "|t" tab
 * marker in the result sets the line's tab flag.
 *
 * Deviation: the decompiler expands the PPC vararg register-homing into explicit fixed params a3..a11 plus
 * spill copies (v17,v19..v23). That is the calling-convention spill of the variadic tail; the true prototype is
 * a single trailing `...`, reconstructed here. */

#include "headers/terminal_globals.h"
#include "headers/output_line_datum.h"
#include "headers/real_argb_color.h"
#include "headers/data_array.h"

#include <stdarg.h>

extern int vsnprintf(char *buffer, unsigned int count, const char *format, va_list args);
extern char *strstr(const char *haystack, const char *needle);
extern int terminal_new_line(void);

void ship_terminal_printf(const real_argb_color *color, const char *format, ...)
{
    static const float default_color[4] = { 1.0f, 0.69999999f, 0.69999999f, 0.69999999f };

    if ( !terminal_globals.initialized )
        return;

    int line_index = terminal_new_line();
    if ( line_index == -1 )
        return;

    output_line_datum *line =
        &((output_line_datum *)terminal_globals.output_lines->data)[(unsigned short)line_index];
    const real_argb_color *use_color = color ? color : (const real_argb_color *)default_color;

    line->timer = 0;   /* DB: output_line_datum.timer @288 (was mislabeled unused) */
    line->color = *use_color;

    va_list args;
    va_start(args, format);
    vsnprintf(line->buffer, 0xFE, format, args);
    va_end(args);

    line->tabstop = strstr(line->buffer, "|t") != 0;
}
