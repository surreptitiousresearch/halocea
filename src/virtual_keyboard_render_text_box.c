/* virtual_keyboard_render_text_box @0x83783A58 — draw the editable text field of the on-screen keyboard:
 * the current buffer text (centered, off-white), an optional full-buffer selection highlight, and a
 * blinking insertion caret. When first_key_replaces_buffer is set the whole buffer is shown selected
 * (a translucent bitmap behind the text). Otherwise, while not selecting, a 1px caret bitmap blinks at
 * 1 Hz, positioned at the insertion point by summing character widths up to the cursor. */

#include <wchar.h>
#include <stdint.h>
#include "headers/virtual_keyboard_globals_t.h"
#include "headers/text_justification.h"
#include "headers/real_argb_color.h"
#include "headers/rectangle2d.h"
#include "headers/bitmap_data.h"
#include "headers/font_header.h"
#include "headers/font_character.h"
#include "headers/global_tag_instances.h"
#include "headers/blam_data_globals.h"


#include "headers/point2d.h"
#include "headers/rasterizer_dynamic_screen_geometry_parameters.h"
extern void draw_string_set_draw_mode(int font_index, int16_t style, int16_t justification, unsigned int flags, const real_argb_color *color);
extern bitmap_data *bitmap_group_get_bitmap_from_sequence(int bitmap_group_index, int16_t sequence_index, int16_t frame_index);
extern void draw_unicode_string_compute_bounds(const rectangle2d *bounds, const wchar_t *string, rectangle2d *text_bounds, rectangle2d *cursor_bounds);
extern void draw_bitmap_in_rect(bitmap_data *bitmap, rectangle2d *rect, rectangle2d *bitmap_rect, rectangle2d *clip_rect, unsigned int argb, rasterizer_dynamic_screen_geometry_parameters *multitexture_params, uint8_t no_plasma);
extern void rasterizer_draw_unicode_string(const rectangle2d *bounds, const rectangle2d *clip, point2d *cursor_reference, int16_t height_adjust, const wchar_t *string);
extern font_character *font_get_character_by_ascii_code(font_header *header, uint16_t character);
extern uint32_t system_milliseconds(void);

void virtual_keyboard_render_text_box(rectangle2d *rect)
{
    real_argb_color text_color;
    text_color.n[0] = 1.0f;
    text_color.n[1] = 0.89999998f;
    text_color.n[2] = 0.89999998f;
    text_color.n[3] = 0.89999998f;

    font_header *text_font = TAG_GET(font_header, virtual_keyboard_globals.text_font_tag);
    draw_string_set_draw_mode(virtual_keyboard_globals.text_font_tag, -1, _text_justification_center, 0, &text_color);

    /* Full-buffer selection highlight. */
    if (virtual_keyboard_globals.first_key_replaces_buffer == 1)
    {
        bitmap_data *highlight = bitmap_group_get_bitmap_from_sequence(virtual_keyboard_globals.caret_bitmap_tag, 0, 0);
        if (highlight)
        {
            rectangle2d text_bounds;
            rectangle2d cursor_bounds;
            draw_unicode_string_compute_bounds(rect, (const wchar_t *)virtual_keyboard_globals.text_buffer,
                                               &text_bounds, &cursor_bounds);
            text_bounds.x0 -= 2;
            text_bounds.x1 += 2;
            draw_bitmap_in_rect(highlight, &text_bounds, rect, nullptr, 0x7F7F7F7F, nullptr, 0);
        }
    }

    rasterizer_draw_unicode_string(rect, rect, nullptr, 0, (const wchar_t *)virtual_keyboard_globals.text_buffer);

    /* Blinking insertion caret (only when not selecting the whole buffer). */
    if (!virtual_keyboard_globals.first_key_replaces_buffer
        && virtual_keyboard_globals.caret_bitmap_tag != -1
        && ((system_milliseconds() / 1000) & 1) != 0)
    {
        __int16 cursor_offset = 0;
        unsigned __int16 *text_buffer = virtual_keyboard_globals.text_buffer;
        __int16 total_width = 0;
        __int16 text_height = text_font->descending_height + text_font->ascending_height;
        bitmap_data *caret = bitmap_group_get_bitmap_from_sequence(virtual_keyboard_globals.caret_bitmap_tag, 0, 0);
        if (caret)
        {
            while (*text_buffer)
            {
                font_character *glyph = font_get_character_by_ascii_code(text_font, *text_buffer);
                if (!glyph)
                    break;
                if (text_buffer < virtual_keyboard_globals.insertion_point)
                    cursor_offset += glyph->character_width;
                ++text_buffer;
                total_width += glyph->character_width;
            }

            int rect_x0 = rect->x0;
            int rect_x1 = rect->x1;
            rectangle2d caret_rect;
            caret_rect.y1 = text_height + 120;
            caret_rect.y0 = 120;
            caret_rect.x0 = (rect_x1 + rect_x0) / 2 - (total_width >> 1) + cursor_offset;
            caret_rect.x1 = caret_rect.x0 + 1;
            draw_bitmap_in_rect(caret, &caret_rect, nullptr, nullptr, 0xFFFFFFFF, nullptr, 0);
        }
    }
}
