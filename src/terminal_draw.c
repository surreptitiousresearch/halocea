/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* terminal_draw @0x836A3428 — render the debug console. Two parts: (1) when a gets() input session is active,
 * draw the prompt+typed-text line (with a blinking insertion-point glyph) along the bottom of the title-safe
 * area; (2) when terminal output is enabled, walk the output-line list newest-first, drawing each line one
 * line-height higher than the last and fading older lines out over time, until the list ends or the top of the
 * screen is reached.
 *
 * The font line height is the sum of three vertical-metric words (ascending+descending+leading) of the interface
 * font definition (DB type font_header). Output-line records are the DB output_line_datum (292 bytes): older_line_index @+8,
 * tabstop @+0xC, buffer @+0xD, color @+0x110, timer @+0x120.
 *
 * DEVIATION: the decompiler reported "local variable allocation has failed". Reconstructed from disassembly. The
 * prompt/result concatenation (inlined strcpy/strcat byte loops) is written as strcpy/strcat; the per-line fade
 * factor (fcfid int->float of the age, then 4 - age/30 clamped to [0,1]) is restored as plain float math. */

#include <stdint.h>
#include "headers/terminal_globals.h"
#include "headers/font_header.h"
#include "headers/output_line_datum.h"
#include "headers/real_argb_color.h"
#include "headers/render_globals.h"
#include "headers/global_tag_instances.h"
#include "headers/point2d.h"
#include "headers/interface_tag_index.h"
#include "headers/text_justification.h"
#include "headers/blam_data_globals.h"
#include "headers/game_time_constants.h"

#include "headers/point2d.h"
extern const __int16 terminal_tab_stops[];

extern char *strcpy(char *destination, const char *source);
extern char *strcat(char *destination, const char *source);
extern unsigned int strlen(const char *string);

extern int interface_get_tag_index(int16_t interface_tag_index);
extern rectangle2d convert_to_title_safe_pixel_bounds(rectangle2d *bounds);
extern rectangle2d *offset_rectangle2d(rectangle2d *rectangle, int16_t dx, int16_t dy);
extern void draw_string_set_draw_mode(int font_index, int16_t style, int16_t justification, unsigned int flags, const real_argb_color *color);
extern void draw_string_set_tab_stops(const int16_t *tab_stops, int16_t count);
extern void rasterizer_draw_string(const rectangle2d *bounds, const rectangle2d *clip, point2d *cursor_reference, int16_t height_adjust, const char *string);

void terminal_draw(void)
{
    int font_index = interface_get_tag_index(_interface_font_terminal);
    if (!terminal_globals.initialized)
        return;

    terminal_gets_state *input_state = terminal_globals.input_state;

    /* font vertical metrics (ascender + descender + leading) give the line height */
    font_header *font_definition = TAG_GET(font_header, font_index);
    /* line height = leading + descending + ascending vertical metrics */
    __int16 line_height = font_definition->leading_height + font_definition->descending_height
                          + (unsigned __int16)font_definition->ascending_height;

    if (input_state)
    {
        char line_buffer[320];
        line_buffer[0] = 0;
        input_state->prompt[31] = 0;
        strcpy(line_buffer, input_state->prompt);
        input_state->result[255] = 0;
        __int16 prompt_length = (__int16)strlen(line_buffer);
        strcat(line_buffer, input_state->result);

        rectangle2d bounds = convert_to_title_safe_pixel_bounds(&render.camera.window_bounds);
        bounds.n[0] = bounds.n[2] - line_height;
        offset_rectangle2d(&bounds, -render.camera.viewport_bounds.n[1], -render.camera.viewport_bounds.n[0]);

        draw_string_set_draw_mode(font_index, -1, _text_justification_left, 0, &terminal_globals.input_state->color);

        if (terminal_globals.insertion_point_visible)
        {
            int cursor_position = (__int16)(terminal_globals.input_state->edit.insertion_point_index + prompt_length);
            if (!line_buffer[cursor_position])
                line_buffer[cursor_position + 1] = 0;
            line_buffer[cursor_position] = 127;
        }

        rasterizer_draw_string(&bounds, nullptr, nullptr, 0, line_buffer);
    }

    if (!terminal_render_enable)
        return;

    rectangle2d bounds = convert_to_title_safe_pixel_bounds(&render.camera.window_bounds);
    __int16 y = bounds.n[2] - line_height;
    int line_index = terminal_globals.newest_output_line_index;
    if (line_index == -1)
        return;

    __int16 right = bounds.n[3];
    __int16 left = bounds.n[1];

    do
    {
        int next_y = y - line_height;
        if (next_y <= 0)
            break;

        output_line_datum *line = &((output_line_datum *)terminal_globals.output_lines->data)[(unsigned __int16)line_index];

        real_argb_color color = line->color;
        int age = line->timer;
        float fade = 4.0f - (float)age * SECONDS_PER_TICK;   /* full for 3s, then fades over 1s */
        if (fade < 0.0f)
            fade = 0.0f;
        else if (fade > 1.0f)
            fade = 1.0f;
        color.n[0] = fade * color.n[0];

        bounds.n[2] = y;
        bounds.n[0] = (__int16)next_y;
        bounds.n[1] = left;
        bounds.n[3] = right;
        y = (__int16)next_y;

        offset_rectangle2d(&bounds, -render.camera.viewport_bounds.n[1], -render.camera.viewport_bounds.n[0]);

        if (line->tabstop)
            draw_string_set_tab_stops(terminal_tab_stops, 3);
        draw_string_set_draw_mode(font_index, -1, _text_justification_left, 0, &color);
        rasterizer_draw_string(&bounds, nullptr, nullptr, 0, line->buffer);
        draw_string_set_tab_stops(terminal_tab_stops, 0);

        line_index = line->older_line_index;
    } while (line_index != -1);
}
