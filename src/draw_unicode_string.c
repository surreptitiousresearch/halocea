/* draw_unicode_string @0x8376AE90 — wide-character counterpart of draw_string: lay out and draw a (markup)
 * Unicode string within a bounding rectangle, honouring the current font-drawing globals (indent / paragraph
 * indent, tab stops, word wrap, justification, vertical clip). Walks the string with parse_unicode_string,
 * measuring glyphs to find line breaks, and draws each line through draw_unicode_string_partial via the
 * supplied per-glyph callback. The final cursor position is written back to cursor_reference. Structurally
 * identical to draw_string (the result-token dispatch is rendered as a clean switch here).
 *
 * DEVIATIONS (disasm-verified): the decompiler rendered the tab-stop count as (__int16)*(_DWORD*)bounds->n;
 * the disassembly loads font_drawing_globals.tab_stop_count there. The line-spacing term shows as
 * HIWORD(v38) — it is just height_adjust (r7) spilled through a stack slot. */

#include <stdint.h>
#include <stddef.h>
#include "headers/parse_string_state.h"
#include "headers/parse_string_result.h"
#include "headers/draw_text_flags.h"
#include "headers/text_justification.h"
#include "headers/font_header.h"
#include "headers/font_character.h"
#include "headers/font_drawing_globals.h"
#include "headers/rectangle2d.h"
#include "headers/point2d.h"
#include "headers/real_argb_color.h"

#include "headers/real_argb_color.h"
extern void parse_string_new(parse_string_state *state, const char *string, int base_font_index, int16_t style, int16_t justification, const real_argb_color *color);
extern uint16_t parse_unicode_string(parse_string_state *state);
extern font_character *font_get_character_by_ascii_code(font_header *header, uint16_t character);
extern int16_t rectangle2d_width(const rectangle2d *rectangle);
extern void draw_unicode_string_partial(void (*draw_character)(parse_string_state *, font_header *, font_character *, unsigned int, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t), point2d *cursor, const rectangle2d *bounds, const rectangle2d *clip, unsigned int color, const wchar_t *string, int16_t string_index, int16_t string_length);

void draw_unicode_string(
    void (*draw_character)(parse_string_state *, font_header *, font_character *, unsigned int, int16_t, int16_t,
                           int16_t, int16_t, int16_t, int16_t),
    const rectangle2d *bounds, point2d *cursor_reference, const rectangle2d *clip, int16_t height_adjust,
    const wchar_t *string)
{
    const rectangle2d *line_bounds = bounds;
    int16_t wrapped_line_index = 0;
    int16_t max_wrapped_line_index = 0;
    int16_t tab_index = 0;
    int16_t paragraph_line_offset = 0;

    parse_string_state state[6];
    parse_string_new(state, (const char *)string, font_drawing_globals.current_font_index,
                     font_drawing_globals.current_style, font_drawing_globals.current_justification,
                     &font_drawing_globals.current_color);

    int16_t string_index = state[0].string_index;
    font_header *font = state[0].font_header;

    point2d cursor;
    while (1)
    {
        int16_t tab_stop_count = font_drawing_globals.tab_stop_count;
        int16_t justification = state[0].justification;
        uint16_t line_width = 0;
        int break_candidate_end = 0;
        int16_t previous_result = -1;
        int16_t segment_start_index = string_index;
        rectangle2d line_rect = *line_bounds;
        char segment_done = 0;
        int segment_end_index = 0;
        uint16_t saved_width_at_break = 0;

        int16_t line_left;
        if (tab_stop_count <= 0)
        {
            int16_t indent = paragraph_line_offset ? font_drawing_globals.paragraph_indent
                                                   : font_drawing_globals.initial_indent;
            line_left = indent + line_rect.n[1];
            line_rect.n[1] += indent;
        }
        else
        {
            if (tab_index)
            {
                /* recovered: (&tab_stop_count)[tab_index] -> tab_stops[tab_index - 1] (contiguous __int16 region) */
                line_left = font_drawing_globals.tab_stops[tab_index - 1];
            }
            else
            {
                int16_t indent = paragraph_line_offset ? font_drawing_globals.paragraph_indent
                                                       : font_drawing_globals.initial_indent;
                line_left = indent + line_rect.n[1];
            }
            line_rect.n[1] = line_left;
            if (tab_index < tab_stop_count)
                line_rect.n[3] = font_drawing_globals.tab_stops[tab_index];
        }

        int16_t ascending_height = font->ascending_height;
        int cursor_x_start = font->leading_width + line_left;
        int line_top = (font->leading_height + font->descending_height + height_adjust + ascending_height)
                            * (wrapped_line_index + paragraph_line_offset)
                        + line_rect.n[0];
        cursor.n[0] = cursor_x_start;
        int baseline_y = line_top + ascending_height;
        cursor.n[1] = line_top + ascending_height;

        /* measure forward to the line break */
        int16_t result;
        do
        {
            char wrapped = 0;
            parse_unicode_string(state);
            result = state[0].result;
            font = state[0].font_header;
            if (result != _parsed_end_of_word && result != _parsed_character)
                goto end_line;
            font_character *character = font_get_character_by_ascii_code(state[0].font_header, state[0].character);
            if (!character)
                goto advance;
            if (result != _parsed_end_of_word && previous_result == _parsed_end_of_word)
            {
                break_candidate_end = segment_end_index;
                saved_width_at_break = line_width;
            }
            if (character->bitmap_width + cursor_x_start + (int16_t)line_width < line_rect.n[3])
            {
                line_width += character->character_width;
                goto advance;
            }
            if (font_drawing_globals.current_flags & (1u << _draw_text_wrap_horizontally_bit))
            {
                if ((int16_t)break_candidate_end > 0)
                {
                    line_width = saved_width_at_break;
                    segment_end_index = break_candidate_end;
                    wrapped = 1;
                }
            end_line:
                segment_done = 1;
            }
        advance:
            if (!wrapped)
                segment_end_index = (uint16_t)state[0].string_index;
            previous_result = result;
        } while (!segment_done);

        /* horizontal justification */
        if (justification == _text_justification_right)
        {
            int16_t width = rectangle2d_width(&line_rect);
            cursor.n[0] = width + line_rect.n[1] - font->leading_width - line_width;
        }
        else if (justification == _text_justification_center)
        {
            int16_t width = rectangle2d_width(&line_rect);
            cursor.n[0] = ((width - (int16_t)line_width) >> 1) + line_rect.n[1];
        }

        /* draw the line unless clipped below the bounds (and not forced) */
        if ((font_drawing_globals.current_flags & (1u << _draw_text_wrap_vertically_bit)) != 0 || baseline_y < line_rect.n[2])
            draw_unicode_string_partial(draw_character, &cursor, &line_rect, clip, state[0].color, string,
                                        segment_start_index, segment_end_index);

        string_index = segment_end_index;
        state[0].string_index = segment_end_index;

        if (result >= _parsed_end_of_line && result <= _parsed_character)
        {
            switch (result)
            {
                case _parsed_end_of_line:   /* paragraph break */
                    wrapped_line_index = 0;
                    tab_index = 0;
                    paragraph_line_offset += max_wrapped_line_index + 1;
                    break;
                case _parsed_end_of_word:   /* end of run → next line */
                default:
                    wrapped_line_index = (int16_t)(wrapped_line_index + 1);
                    if (wrapped_line_index > max_wrapped_line_index)
                        max_wrapped_line_index = wrapped_line_index;
                    break;
                case _parsed_end_of_column:   /* tab */
                    if (tab_index < font_drawing_globals.tab_stop_count)
                    {
                        wrapped_line_index = 0;
                        ++tab_index;
                    }
                    break;
                case _parsed_justification_change:
                    wrapped_line_index = 0;
                    break;
                case _parsed_color_change:
                    break;
            }
        }

        if (result)
        {
            line_bounds = bounds;
            continue;
        }
        break;
    }

    font_drawing_globals.highlight_stop_index = 0;
    font_drawing_globals.highlight_start_index = 0;
    if (cursor_reference)
        *cursor_reference = cursor;
}
