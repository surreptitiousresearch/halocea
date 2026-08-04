/* terminal_remove_line @0x836A3000 — unlink a terminal output line from the doubly-linked list (fixing up the
 * neighbours' links or the oldest/newest indices) and delete its datum.
 *
 * Records are the DB output_line_datum (292 bytes): newer_line_index @+4 (dword 1), older_line_index @+8
 * (dword 2). Raw 73-dword walk in the disassembly is typed through the DB struct here. */

#include <stdint.h>
#include "headers/terminal_globals.h"
#include "headers/data_array.h"
#include "headers/output_line_datum.h"

extern void datum_delete(data_array *data, int index);

void terminal_remove_line(uint16_t line_index)
{
    data_array *output_lines = terminal_globals.output_lines;
    output_line_datum *lines = (output_line_datum *)output_lines->data;
    output_line_datum *line = &lines[line_index];

    int older = line->older_line_index;
    int newer = line->newer_line_index;

    if ( older == -1 )
        terminal_globals.oldest_output_line_index = newer;
    else
        lines[(unsigned short)older].newer_line_index = newer;

    if ( newer == -1 )
        terminal_globals.newest_output_line_index = older;
    else
        lines[(unsigned short)newer].older_line_index = older;

    datum_delete(output_lines, line_index);
}
