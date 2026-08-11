/* terminal_update_output @0x836A3260 — ages the console output lines once per frame. Each line record
 * (DB output_line_datum, 292 bytes) carries a timer; once a line has been shown > 150 frames it is
 * unlinked from the newest<->oldest doubly-linked list and its datum freed. Records are walked from
 * newest toward oldest via older_line_index (@+8). newer_line_index is @+4, timer is @+288. */

#include <stdint.h>
#include "headers/terminal_globals.h"
#include "headers/output_line_datum.h"

extern void datum_delete(data_array *data, int index);

void terminal_update_output(void)
{
    int line_index = terminal_globals.newest_output_line_index;
    if (line_index == -1)
        return;

    data_array *output_lines = terminal_globals.output_lines;
    int next_index;
    do
    {
        output_line_datum *lines = (output_line_datum *)output_lines->data;
        output_line_datum *line = &lines[(uint16_t)line_index];
        next_index = line->older_line_index;

        int age = line->timer + 1;
        line->timer = age;
        if (age > 150)
        {
            int older = line->older_line_index;
            int newer = line->newer_line_index;

            /* detach from the older neighbour (or update the oldest pointer) */
            if (older == -1)
                terminal_globals.oldest_output_line_index = newer;
            else
                lines[(uint16_t)older].newer_line_index = newer;

            /* detach from the newer neighbour (or update the newest pointer) */
            if (newer == -1)
                terminal_globals.newest_output_line_index = older;
            else
                lines[(uint16_t)newer].older_line_index = older;

            datum_delete(output_lines, line_index);
            output_lines = terminal_globals.output_lines;
        }
        line_index = next_index;
    }
    while (next_index != -1);
}
