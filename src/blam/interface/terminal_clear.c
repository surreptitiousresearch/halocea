/* terminal_clear @0x836A3078 — clear the in-game terminal output (reset the line indices and delete all
 * output-line data), if the terminal is initialized. */

#include "headers/terminal_globals.h"
#include "headers/data_array.h"

#include "headers/data_array.h"
extern void data_delete_all(data_array *data);

void terminal_clear(void)
{
    if ( terminal_globals.initialized )
    {
        terminal_globals.newest_output_line_index = -1;
        terminal_globals.oldest_output_line_index = -1;
        data_delete_all(terminal_globals.output_lines);
    }
}
