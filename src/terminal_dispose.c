/* terminal_dispose @0x836A2F98 — one-time shutdown of the developer terminal: invalidate and free the
 * output-line datum array, then mark the terminal uninitialized. */

#include "headers/terminal_globals.h"

#include "headers/data_array.h"
#include "headers/data_array.h"
extern void data_make_invalid(data_array *data);
extern void data_dispose(data_array *data);

void terminal_dispose(void)
{
    if ( terminal_globals.output_lines )
    {
        if ( terminal_globals.output_lines->valid )
            data_make_invalid(terminal_globals.output_lines);
        data_dispose(terminal_globals.output_lines);
    }
    terminal_globals.initialized = 0;
}
