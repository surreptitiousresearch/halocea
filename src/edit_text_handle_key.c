/* edit_text_handle_key 0x83799A08 — apply one keystroke to a text-edit control. Handles delete/backspace
 * (_key_backspace / _key_delete), printable character insertion (ascii >= 0x20, != 0xFF), and left/right caret
 * movement (_key_left_arrow / _key_right_arrow) with shift-to-select. A live selection is replaced/deleted as a unit; otherwise edits
 * happen at the caret. Every path ends by realigning the caret to a character boundary. Character boundaries are
 * resolved with get_previous_character / get_next_character so multi-byte characters move and delete atomically.
 *
 * Faithful note: edit_text_get_selection_indices fills (start_index, end_index); the delete and insert paths
 * pass the two scratch slots in opposite order, so index_a/index_b swap roles between branches as in the original. */

#include <stdint.h>
#include "headers/edit_text.h"
#include "headers/key_modifier_flags.h"
#include "headers/key_stroke.h"
#include "headers/key_code.h"

extern void edit_text_fix_selection(edit_text *edit);
extern uint8_t edit_text_get_selection_indices(edit_text *edit, int16_t *start_index, int16_t *end_index);
extern uint16_t get_previous_character(const unsigned char *string, int16_t *index);
extern uint16_t get_next_character(const unsigned char *string, int16_t *index);
extern void align_to_character(const unsigned char *string, int16_t *index);
extern void *memmove(void *dest, const void *src, unsigned int count);

void edit_text_handle_key(edit_text *edit, const key_stroke *key)
{
    edit_text_fix_selection(edit);

    int key_code = key->key_code;
    __int16 index_a;  /* sp+50 scratch index */
    __int16 index_b;  /* sp+52 scratch index */

    if ( key_code == _key_backspace || key_code == _key_delete )
    {
        if ( !edit_text_get_selection_indices(edit, &index_a, &index_b) )  /* no selection */
        {
            char *tail;
            char *tail_end;
            __int16 destination_index;

            if ( key->key_code == _key_backspace && edit->insertion_point_index > 0 )
            {
                __int16 old_index = edit->insertion_point_index;
                get_previous_character((unsigned __int8 *)edit->buffer, &edit->insertion_point_index);
                tail = &edit->buffer[old_index];
                tail_end = tail;
                while ( *tail_end++ )
                    ;
                destination_index = edit->insertion_point_index;
            }
            else  /* forward delete */
            {
                unsigned __int8 *buffer = (unsigned __int8 *)edit->buffer;
                char *end = edit->buffer;
                while ( *end++ )
                    ;
                if ( key->key_code != _key_delete
                     || edit->insertion_point_index >= (unsigned int)(end - (char *)buffer - 1) )
                {
                    align_to_character((unsigned __int8 *)edit->buffer, &edit->insertion_point_index);
                    return;
                }
                index_a = edit->insertion_point_index;
                get_next_character(buffer, &index_a);
                tail = &edit->buffer[index_a];
                tail_end = tail;
                while ( *tail_end++ )
                    ;
                destination_index = edit->insertion_point_index;
            }
            memmove(&edit->buffer[destination_index], tail, tail_end - tail);
            align_to_character((unsigned __int8 *)edit->buffer, &edit->insertion_point_index);
            return;
        }

        /* delete the selected range [index_a, index_b) by shifting the tail down */
        char *tail = &edit->buffer[index_b];
        while ( *tail++ )
            ;
        __int16 selection_start = index_a;
        memmove(&edit->buffer[index_a], &edit->buffer[index_b], tail - &edit->buffer[index_b]);
        edit->insertion_point_index = selection_start;
        edit->selection_start_index = -1;
        align_to_character((unsigned __int8 *)edit->buffer, &edit->insertion_point_index);
        return;
    }

    if ( key_code != _key_left_arrow && key_code != _key_right_arrow )  /* not an arrow key: character insertion */
    {
        unsigned int ascii_code = key->ascii_code;
        if ( ascii_code >= 0x20 && ascii_code != 255 )
        {
            unsigned __int8 has_selection = edit_text_get_selection_indices(edit, &index_b, &index_a);
            char *buffer = edit->buffer;
            if ( has_selection )  /* replace selection with the typed character */
            {
                char *tail = &buffer[index_a];
                while ( *tail++ )
                    ;
                __int16 selection_start = index_b;
                memmove(&buffer[index_b + 1], &buffer[index_a], tail - &buffer[index_a]);
                edit->insertion_point_index = selection_start;
                edit->selection_start_index = -1;
                edit->buffer[selection_start] = key->ascii_code;
                ++edit->insertion_point_index;
                align_to_character((unsigned __int8 *)edit->buffer, &edit->insertion_point_index);
                return;
            }

            /* no selection: insert the character at the caret if it fits */
            char *end = edit->buffer;
            while ( *end++ )
                ;
            if ( (unsigned int)(end - buffer - 1) < (unsigned int)edit->maximum_length )
            {
                char *at_caret = &buffer[edit->insertion_point_index];
                char *at_caret_end = at_caret;
                while ( *at_caret_end++ )
                    ;
                memmove(at_caret + 1, at_caret, at_caret_end - at_caret);
                edit->buffer[edit->insertion_point_index] = key->ascii_code;
                ++edit->insertion_point_index;
                align_to_character((unsigned __int8 *)edit->buffer, &edit->insertion_point_index);
                return;
            }
        }
        align_to_character((unsigned __int8 *)edit->buffer, &edit->insertion_point_index);
        return;
    }

    /* left (79) / right (80) arrow */
    if ( (key->modifier_flags & (1u << _key_modifier_shift_bit)) != 0 || !edit_text_get_selection_indices(edit, &index_b, &index_a) )
    {
        /* shift held or no selection: move the caret, extending the selection when shift is held */
        if ( (key->modifier_flags & (1u << _key_modifier_shift_bit)) != 0 && (unsigned __int16)edit->selection_start_index == 0xFFFF )
            edit->selection_start_index = edit->insertion_point_index;

        int code = key->key_code;
        if ( code == _key_left_arrow && edit->insertion_point_index > 0 )
        {
            get_previous_character((unsigned __int8 *)edit->buffer, &edit->insertion_point_index);
        }
        else if ( code == _key_right_arrow )
        {
            unsigned __int8 *buffer = (unsigned __int8 *)edit->buffer;
            char *end = edit->buffer;
            while ( *end++ )
                ;
            if ( edit->insertion_point_index < (unsigned int)(end - (char *)buffer - 1) )
                get_next_character(buffer, &edit->insertion_point_index);
        }

        if ( edit->selection_start_index == edit->insertion_point_index )
            edit->selection_start_index = -1;
        align_to_character((unsigned __int8 *)edit->buffer, &edit->insertion_point_index);
        return;
    }

    /* has selection, no shift: collapse the selection toward the arrow's direction */
    int code = (unsigned __int16)key->key_code;
    edit->selection_start_index = -1;
    if ( code == _key_left_arrow )
        edit->insertion_point_index = index_b;  /* left -> selection start */
    else
        edit->insertion_point_index = index_a;  /* right -> selection end */
    align_to_character((unsigned __int8 *)edit->buffer, &edit->insertion_point_index);
}
