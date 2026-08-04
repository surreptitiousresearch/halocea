/* hs_help @0x83730160 — print a HaloScript function's signature and documentation to the console, splitting
 * the documentation on newlines so each line is printed separately.
 *
 * DEVIATION: the documentation copy is an inline byte loop in the binary; reproduced as strcpy. */

#include <stdint.h>
#include <string.h>
#include "headers/hs_function_definition.h"
#include "headers/blam_data_globals.h"

extern int16_t hs_find_function_by_name(const char *name);
extern void hs_get_function_parameters_string(int16_t function_index, char *buffer);
extern void ship_terminal_printf(const real_argb_color *color, const char *format, ...);

void hs_help(const char *function_name)
{
    __int16 function_index = hs_find_function_by_name(function_name);
    if ( function_index == -1 )
        return;

    char buffer[2080];
    hs_get_function_parameters_string(function_index, buffer);
    ship_terminal_printf(nullptr, buffer);
    strcpy(buffer, hs_function_table[function_index]->documentation);

    char *newline = strchr(buffer, '\n');
    const char *line = buffer;
    if ( newline )
    {
        while ( newline )
        {
            *newline = 0;
            ship_terminal_printf(nullptr, line);
            line = newline + 1;
            newline = strchr(line, '\n');
        }
    }
    ship_terminal_printf(nullptr, line);
}
