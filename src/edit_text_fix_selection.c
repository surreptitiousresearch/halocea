/* edit_text_fix_selection @ 0x837995F0 — clamp the caret and selection-anchor indices to the
 * current string length (caret to [0,len], anchor to [-1,len]); collapse an empty selection to
 * -1; and snap both to character boundaries. */

#include <stdint.h>
#include "headers/console_globals.h"

extern void align_to_character(const unsigned char *string, int16_t *index);

void edit_text_fix_selection(edit_text *edit)
{
    const unsigned char *buffer = (const unsigned char *)edit->buffer;
    const unsigned char *end = buffer;
    int16_t length;
    int16_t caret;
    int16_t anchor;

    while ( *end++ )
        ;
    length = (int16_t)(end - buffer - 1);

    caret = edit->insertion_point_index;
    if ( caret >= 0 )
    {
        if ( caret > length )
            caret = length;
    }
    else
    {
        caret = 0;
    }
    edit->insertion_point_index = caret;

    anchor = edit->selection_start_index;
    if ( anchor >= -1 )
    {
        if ( anchor > length )
            anchor = length;
    }
    else
    {
        anchor = -1;
    }
    edit->selection_start_index = anchor;

    if ( caret == anchor )
        edit->selection_start_index = -1;

    align_to_character(buffer, &edit->insertion_point_index);
    if ( (uint16_t)edit->selection_start_index != 0xFFFF )
        align_to_character((const unsigned char *)edit->buffer, &edit->selection_start_index);
}
