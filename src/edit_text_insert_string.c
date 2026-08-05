/* edit_text_insert_string @ 0x837997B8 — insert text into an edit field at the caret. If there
 * is an active selection the selected range is replaced; otherwise the text is inserted at the
 * caret provided it fits within maximum_length. The caret is left after the inserted text and
 * snapped to a character boundary. */

#include <stdint.h>
#include "headers/console_globals.h"

extern uint8_t edit_text_get_selection_indices(edit_text *edit, int16_t *start_index, int16_t *end_index);
extern void *memmove(void *dst, const void *src, unsigned int n);
extern void align_to_character(const unsigned char *string, int16_t *index);

static int string_length(const char *s)
{
    const char *p = s;
    while ( *p++ )
        ;
    return (int)(p - s - 1);
}

void edit_text_insert_string(edit_text *edit, const char *string)
{
    int16_t selection_end;
    int16_t selection_start;

    if ( edit_text_get_selection_indices(edit, &selection_start, &selection_end) )
    {
        /* replace the selected range [selection_start, selection_end) */
        char *tail = &edit->buffer[selection_end];
        int tail_length = string_length(tail);
        int insert_length = string_length(string);

        memmove(&edit->buffer[selection_start - 1 + insert_length], tail, tail_length + 1);
        edit->insertion_point_index = selection_start;
        edit->selection_start_index = -1;
        while ( *string )
            edit->buffer[edit->insertion_point_index++] = *string++;
    }
    else
    {
        int current_length = string_length(edit->buffer);
        int insert_length = string_length(string);

        if ( (unsigned int)(insert_length + current_length) < (unsigned int)edit->maximum_length )
        {
            char *at_caret = &edit->buffer[edit->insertion_point_index];
            int after_caret_length = string_length(at_caret);

            memmove(&at_caret[insert_length], at_caret, after_caret_length + 1);
            while ( *string )
                edit->buffer[edit->insertion_point_index++] = *string++;
        }
    }

    align_to_character((const unsigned char *)edit->buffer, &edit->insertion_point_index);
}
