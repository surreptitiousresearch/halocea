/* parse_unicode_string @0x8376A378 — decode the next UTF-16 character/control token from a parse_string
 * cursor. Returns a parse_string_result (also stored in state->result): end-of-string, end-of-line (newline),
 * end-of-column (tab), or _parsed_character for an ordinary printable character. The '|' (0x7C) char
 * introduces a control escape; "|n" maps to a newline (_parsed_end_of_line, character forced to CR/13),
 * any other "|x" backs up and treats '|' as a literal character. */

#include <stdint.h>
#include "headers/parse_string_state.h"
#include "headers/parse_string_result.h"

/* Attestation: all 6 return paths end in clrlwi r3,rX,16 (explicit 16-bit zero-extend), so the
 * declared return is uint16_t even though the sampled callers ignore the value. */
uint16_t parse_unicode_string(parse_string_state *state)
{
    unsigned __int8 *string = state->string;
    int string_index = state->string_index;

    int character = *(unsigned __int16 *)&string[2 * string_index];
    state->string_index = string_index + 1;
    state->character = character;

    if (character <= 13)
    {
        switch (character)
        {
        case '\r':
            state->result = _parsed_end_of_line;
            return _parsed_end_of_line;
        case '\0':
            state->result = _parsed_end_of_string;
            return _parsed_end_of_string;
        case '\t':
            state->result = _parsed_end_of_column;
            return _parsed_end_of_column;
        }
        state->result = _parsed_character;
        return _parsed_character;
    }

    if (character != 124)
    {
        state->result = _parsed_character;
        return _parsed_character;
    }

    int next_character = *(unsigned __int16 *)&string[2 * state->string_index];
    __int16 advanced_index = state->string_index + 1;
    state->string_index = advanced_index;
    if (next_character == 110)
    {
        state->result = _parsed_end_of_line;
        state->character = 13;
        return _parsed_end_of_line;
    }
    else
    {
        state->result = _parsed_character;
        state->string_index = advanced_index - 1;
        return _parsed_character;
    }
}
