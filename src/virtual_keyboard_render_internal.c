/* virtual_keyboard_render_internal @0x83783C88 — draw the on-screen virtual keyboard: the background
 * bitmap (if present), the caption string for the current special-key set, the static "done" label
 * (special key 14), and the editable text box. All text is drawn in the off-white tint {1.0,0.9,0.9,0.9}
 * with the keyboard's caption font.
 *
 * DEVIATION: the decompiler fuses each rectangle2d into a packed __int64 and shows the rect corners as
 * opaque hex constants. Restored from the disassembly, the corners are {y0,x0,y1,x1} stored as four
 * shorts; each rect is given explicit named fields below. The shared color buffer is built once and
 * copied into the argb temp passed to draw_string_set_draw_mode. */

#include <wchar.h>
#include <stdint.h>
#include "headers/virtual_keyboard_globals_t.h"
#include "headers/text_justification.h"
#include "headers/real_argb_color.h"
#include "headers/rectangle2d.h"
#include "headers/bitmap_data.h"
#include "headers/blam_data_globals.h"


#include "headers/point2d.h"
#include "headers/rasterizer_dynamic_screen_geometry_parameters.h"
extern bitmap_data *bitmap_group_get_bitmap_from_sequence(int bitmap_group_index, int16_t sequence_index, int16_t frame_index);
extern void draw_bitmap_in_rect(bitmap_data *bitmap, rectangle2d *rect, rectangle2d *bitmap_rect, rectangle2d *clip_rect, unsigned int argb, rasterizer_dynamic_screen_geometry_parameters *multitexture_params, uint8_t no_plasma);
extern void draw_string_set_draw_mode(int font_index, int16_t style, int16_t justification, unsigned int flags, const real_argb_color *color);
extern unsigned short *unicode_string_list_get_string(int tag_index, int16_t string_index);
extern void rasterizer_draw_unicode_string(const rectangle2d *bounds, const rectangle2d *clip, point2d *cursor_reference, int16_t height_adjust, const wchar_t *string);
extern void virtual_keyboard_render_text_box(rectangle2d *rect);

void virtual_keyboard_render_internal(void)
{
    real_argb_color text_color;
    text_color.n[0] = 1.0f;
    text_color.n[1] = 0.89999998f;
    text_color.n[2] = 0.89999998f;
    text_color.n[3] = 0.89999998f;

    /* Background bitmap, full screen. */
    if (virtual_keyboard_globals.keyboard->background_bitmap_tag.index != -1)
    {
        rectangle2d background_rect = { 0, 0, 480, 640 };
        bitmap_data *background = bitmap_group_get_bitmap_from_sequence(
            virtual_keyboard_globals.keyboard->background_bitmap_tag.index, 0, 0);
        draw_bitmap_in_rect(background, &background_rect, &background_rect, nullptr, 0xFFFFFFFF, nullptr, 0);
    }

    draw_string_set_draw_mode(virtual_keyboard_globals.caption_font_tag, -1, _text_justification_left, 0, &text_color);

    virtual_keyboard *keyboard = virtual_keyboard_globals.keyboard;

    /* Caption for the current special-key set. */
    if (keyboard->special_key_labels_string_list_tag.index != -1)
    {
        const wchar_t *caption = unicode_string_list_get_string(
            keyboard->special_key_labels_string_list_tag.index, virtual_keyboard_globals.caption_index);
        rectangle2d caption_rect = { 78, 114, 110, 640 };
        rasterizer_draw_unicode_string(&caption_rect, &caption_rect, nullptr, 0, caption);
        keyboard = virtual_keyboard_globals.keyboard;
    }

    /* Static "done" label (special key 14), left-justified. */
    const wchar_t *done_label = unicode_string_list_get_string(
        keyboard->special_key_labels_string_list_tag.index, 14);
    rectangle2d done_rect = { 414, 0, 450, 630 };
    draw_string_set_draw_mode(virtual_keyboard_globals.caption_font_tag, -1, _text_justification_right, 0, &text_color);
    rasterizer_draw_unicode_string(&done_rect, &done_rect, nullptr, 0, done_label);

    /* Editable text box. */
    rectangle2d text_box_rect = { 118, 120, 143, 520 };
    virtual_keyboard_render_text_box(&text_box_rect);
}
