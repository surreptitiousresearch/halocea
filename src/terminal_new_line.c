/* terminal_new_line @0x836A37A0 — allocate a new terminal output-line datum, evicting the oldest line when the
 * 32-line buffer is full, and link it as the newest line in the doubly-linked list of output lines. Returns
 * the new line index. */

#include <stdint.h>
#include "headers/terminal_globals.h"
#include "headers/output_line_datum.h"
#include "headers/data_array.h"

extern void terminal_remove_line(uint16_t line_index);
extern int datum_new(data_array *data);

int terminal_new_line(void)
{
    data_array *output_lines = terminal_globals.output_lines;
    if ( output_lines->count == 32 )
    {
        terminal_remove_line(terminal_globals.oldest_output_line_index);
        output_lines = terminal_globals.output_lines;
    }

    int new_index = datum_new(output_lines);
    int previous_newest = terminal_globals.newest_output_line_index;
    terminal_globals.newest_output_line_index = new_index;

    output_line_datum *line =
        &((output_line_datum *)terminal_globals.output_lines->data)[(unsigned short)new_index];
    line->older_line_index = previous_newest;
    line->newer_line_index = -1;

    if ( previous_newest == -1 )
        terminal_globals.oldest_output_line_index = new_index;
    else
        ((output_line_datum *)terminal_globals.output_lines->data)[(unsigned short)previous_newest].newer_line_index = new_index;

    return new_index;
}
