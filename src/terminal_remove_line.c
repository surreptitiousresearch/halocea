/* terminal_remove_line @0x836A2FF0 — unlink a terminal output line from the doubly-linked list (fixing up the
 * neighbours' links or the oldest/newest indices) and delete its datum.
 *
 * Records are the DB output_line_datum (292 bytes): newer_line_index @+4 (dword 1), older_line_index @+8
 * (dword 2). Raw 73-dword walk in the disassembly is typed through the DB struct here. */

#include <stdint.h>
#include "headers/terminal_globals.h"
#include "headers/data_array.h"
#include "headers/output_line_datum.h"

extern void datum_delete(data_array *data, int index);

/* DEVIATION (2026-08-12, #134): line_index is `int`, a full datum handle, NOT the `uint16_t` this was
 * declared with. `mr r4, r3 # index` @0x836A3004 copies the parameter WHOLE into datum_delete's index
 * argument and r4 is never rewritten before `b datum_delete` @0x836A3070; datum_delete's salt gate
 * @0x836FB50C-18 (`cmpwi cr6,r9,0 / beq loc_836FB524`) skips the identifier compare when the salt is
 * zero, which a zero-extended uint16_t guarantees. The caller loads a full word:
 * `lwz r3, terminal_globals.oldest_output_line_index` @0x836A37B0 -> `bl terminal_remove_line`, and
 * that global is an `int` filled straight from datum_new's salted return (`stw r3, ...` @0x836A37D0).
 *
 * The `(uint16_t)` on the local subscript below is therefore load-bearing, and is the one BODY edit
 * this widening implies anywhere in the campaign: the binary masks with `clrlwi r10, r3, 16`
 * @0x836A2FF4 before `mulli r11, r10, 0x124`, and this TU hand-rolls its element address instead of
 * going through DATA_ARRAY_ELEMENT (which truncates internally). Without the cast the widened handle
 * would subscript with the salt still in the high half. The two neighbour subscripts below already
 * spell the same truncation. */
void terminal_remove_line(int line_index)
{
    data_array *output_lines = terminal_globals.output_lines;
    output_line_datum *lines = (output_line_datum *)output_lines->data;
    output_line_datum *line = &lines[(unsigned short)line_index];

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
