/* edit_text_get_selection_indices @0x83799754 — output the ordered [start, end] character indices of
 * the current selection, returning 0 when there is no selection. start is the lower of the selection
 * anchor and the insertion point; end is the higher. */

#include <stdint.h>
#include "headers/edit_text.h"

extern void edit_text_fix_selection(edit_text *edit);

/* return attested uint8_t: 4/4 callers clrlwi r3,24 */
uint8_t edit_text_get_selection_indices(edit_text *edit, int16_t *start_index, int16_t *end_index)
{
    edit_text_fix_selection(edit);
    if ( edit->selection_start_index == -1 )
        return 0;

    int16_t start = edit->selection_start_index;
    if ( start > edit->insertion_point_index )
        start = edit->insertion_point_index;
    *start_index = start;

    int16_t end = edit->selection_start_index;
    if ( end <= edit->insertion_point_index )
        end = edit->insertion_point_index;
    *end_index = end;
    return 1;
}
