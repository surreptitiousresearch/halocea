/* edit_text_selection_reset @ 0x837996E0 — collapse the selection and move the caret
 * to the end of the edit buffer's current string. */

#include "headers/console_globals.h"

extern void edit_text_fix_selection(edit_text *edit);

void edit_text_selection_reset(edit_text *edit)
{
    char *cursor;
    int length;

    edit_text_fix_selection(edit);
    cursor = edit->buffer;
    while ( *cursor++ )
        ;
    length = (int)(cursor - edit->buffer);
    edit->selection_start_index = -1;
    edit->insertion_point_index = length - 1;
}
