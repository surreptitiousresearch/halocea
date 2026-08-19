/* terminal_initialize @ 0x836A2F20 — allocate the debug terminal output buffer */

#include <stdint.h>
#include "headers/data_array.h"

extern data_array *data_new(const char *name, int16_t maximum_count, int16_t size);
extern void data_make_valid(data_array *data);

#include "headers/terminal_globals.h"
#include "headers/blam_data_globals.h"
/* DEVIATION: the decompiler ordered the fields {input_state@8, newest@0xC, oldest@0x10, last_toggle@0x14};
 * the DB-verified layout (terminal_globals.h) is {newest@8, oldest@0xC, input_state@0x10, last_toggle@0x18}.
 * Reusing the canonical header corrects the offsets (field names match). */

void terminal_initialize(void)
{
    data_array *lines = data_new("terminal output", 32, 292);
    terminal_globals.initialized = 1;
    terminal_globals.output_lines = lines;
    data_make_valid(lines);
    terminal_globals.newest_output_line_index = -1;
    terminal_globals.input_state = 0;
    terminal_globals.oldest_output_line_index = -1;
    terminal_globals.last_insertion_point_toggle = 0;
    terminal_render_enable = 1;
}
