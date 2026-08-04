/* edit_text_new @ 0x837999B0 — initialize an edit field: fix up the selection then place the
 * caret at the end of the current buffer text. (Identical body to edit_text_selection_reset.) */

#include "headers/console_globals.h"

extern void edit_text_fix_selection(edit_text *edit);

void edit_text_new(edit_text *edit)
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
