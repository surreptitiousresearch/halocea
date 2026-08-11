/* parse_string @0x8376A0B8 — advance the markup string cursor by one token and classify it. Returns a result
 * code stored in state->result: 0 end-of-string, 1 newline, 2 printable character, 3 tab, 4 justification
 * change, 6 whitespace/word-break. Style tokens (|b |i |k |p |u, result 7) update the active font and loop to
 * read the following token, so they are never returned. The '|xx' escape uses 0x7C in the high byte.
 *
 * NOTE: DB enum $403D44D8579E3FC5128F842C44A4E0BF (_parsed_*) names the parse-string result codes, but this
 * ASCII path's numeric assignments do NOT match it (here 2=printable char and 6=word-break, whereas the enum
 * has 2=end_of_word, 6=character; 3=tab vs enum 3=end_of_column). Left as raw literals rather than binding
 * names that would read with inverted meaning. Only the justification literals (which match text_justification
 * cleanly) are named. */

#include <stdint.h>
#include "headers/parse_string_state.h"
#include "headers/font_drawing_globals.h"
#include "headers/font_header.h"
#include "headers/global_tag_instances.h"
#include "headers/text_justification.h"

extern int tolower(int c);
extern uint16_t get_next_character(const unsigned char *string, int16_t *index);
extern char *string_list_get_string(int tag_index, int16_t string_index);
extern uint8_t character_in_pattern(uint16_t character, char *pattern);

int16_t parse_string(parse_string_state *state)
{
    int16_t *string_index = &state->string_index;
    uint16_t next_character;
    int result;

    do
    {
        next_character = get_next_character(state->string, string_index);
        result = -1;

        if ((next_character & 0xFF00) == 0x7C00)   /* "|x" markup escape */
        {
            switch (tolower((uint8_t)next_character))
            {
                case 'b': result = 7; state->style = 0;  break;
                case 'c': result = 4; state->justification = _text_justification_center; break;
                case 'i': result = 7; state->style = 1;  break;
                case 'k': result = 7; state->style = 2;  break;
                case 'l': result = 4; state->justification = _text_justification_left; break;
                case 'n': result = 1; break;
                case 'p': result = 7; state->style = -1; break;
                case 'r': result = 4; state->justification = _text_justification_right; break;
                case 't': result = 3; break;
                case 'u': result = 7; state->style = 3;  break;
                default: break;
            }
        }

        if ((int16_t)result == -1)
        {
            switch (next_character)
            {
                case '\0': result = 0; continue;   /* end of string */
                case '\t': result = 3; continue;   /* tab */
                case '\r': result = 1; continue;   /* carriage return / newline */
            }

            uint8_t *string = state->string;
            int16_t peek_index = *string_index;
            uint16_t following_character = get_next_character(string, &peek_index);
            char *whitespace_pattern = string_list_get_string(font_drawing_globals.string_list_index, 4);
            char *break_before_pattern = string_list_get_string(font_drawing_globals.string_list_index, 5);
            char *break_after_pattern = string_list_get_string(font_drawing_globals.string_list_index, 6);

            if ((next_character & 0xFF00) != 0)
                goto check_break_before;
            if (character_in_pattern(next_character, whitespace_pattern))
            {
                result = 6;
                continue;
            }
            if ((next_character & 0xFF00) != 0)
            {
            check_break_before:
                if (character_in_pattern(next_character, break_before_pattern))
                {
                    result = 6;
                    continue;
                }
            }
            if (character_in_pattern(following_character, break_after_pattern))
            {
                result = 6;
                continue;
            }
            result = 2;   /* ordinary printable character */
        }
        else if ((int16_t)result == 7)
        {
            int style = state->style;
            int font_index;
            if (style == -1)
                font_index = state->base_font_index;
            else
                font_index = TAG_GET(int, state->base_font_index)[4 * style + 18];
            if (font_index == -1)
                font_index = state->base_font_index;
            state->font_header = TAG_GET(font_header, font_index);
        }
    }
    while ((int16_t)result == 7 || (int16_t)result == 5);

    state->character = next_character;
    state->result = result;
    return result;
}
