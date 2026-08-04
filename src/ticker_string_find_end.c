/* ticker_string_find_end @0x8376A990 — find how much of `string` fits within `*max_len` pixels (for a
 * scrolling ticker), accumulating glyph widths as it parses. Control tokens (_parsed_end_of_line.._parsed_color_change) always advance the returned string_index without consuming width. A
 * plain character either fits (advances string_index, accumulates its width) or doesn't (stops, and
 * *max_len is reduced by the width already consumed so the caller knows how much was used). Parsing also
 * stops at end-of-string (result 0), reporting string_index 0 and consumed width via *max_len. Returns the
 * string_index of the first character that didn't fit (or 0 at end-of-string). */

#include <stdint.h>
#include <stddef.h>   /* wchar_t */
#include "headers/parse_string_state.h"
#include "headers/parse_string_result.h"
#include "headers/font_drawing_globals.h"
#include "headers/font_character.h"
#include "headers/real_argb_color.h"

#include "headers/real_argb_color.h"
extern void parse_string_new(parse_string_state *state, const char *string, int base_font_index, int16_t style, int16_t justification, const real_argb_color *color);
extern uint16_t parse_unicode_string(parse_string_state *state);
extern font_character *font_get_character_by_ascii_code(font_header *header, uint16_t character);

int ticker_string_find_end(const wchar_t *string, int *max_len)
{
    unsigned __int8 done = 0;
    int consumed_width = 0;
    int string_index = 0;

    parse_string_state state;
    parse_string_new(&state, (const char *)string, font_drawing_globals.current_font_index,
        font_drawing_globals.current_style, font_drawing_globals.current_justification,
        &font_drawing_globals.current_color);

    do
    {
        parse_unicode_string(&state);

        if ( (unsigned int)state.result > 6 )
            continue;

        if ( !state.result )
        {
            string_index = 0;
            done = 1;
            *max_len -= consumed_width;
            continue;
        }

        if ( state.result >= _parsed_end_of_line && state.result <= _parsed_color_change )
        {
            string_index = state.string_index;
            continue;
        }

        font_character *glyph = font_get_character_by_ascii_code(state.font_header, state.character);
        if ( !glyph )
            continue;

        if ( glyph->bitmap_width + consumed_width >= *max_len )
        {
            done = 1;
            *max_len -= consumed_width;
        }
        else
        {
            string_index = state.string_index;
            consumed_width += glyph->character_width;
        }
    }
    while ( !done );

    return string_index;
}
