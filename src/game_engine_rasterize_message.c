/* game_engine_rasterize_message @0x837480A0 */
#include <wchar.h>
#include <stdint.h>
#include "headers/real_argb_color.h"
#include "headers/text_justification.h"
#include "headers/rectangle2d.h"
#include "headers/interface_tag_index.h"
#include "headers/draw_text_flags.h"
#include "headers/blam_data_globals.h"

#include "headers/point2d.h"
extern int interface_get_tag_index(int16_t interface_tag_index);
extern void draw_string_set_draw_mode(int font_index, int16_t style, int16_t justification, unsigned int flags, const real_argb_color *color);
extern void draw_string_set_color(const real_argb_color *color);
extern void rasterizer_draw_unicode_string(const rectangle2d *bounds, const rectangle2d *clip, point2d *cursor_reference, int16_t height_adjust, const wchar_t *string);
extern void draw_string_set_format(int16_t style, int16_t justification, unsigned int flags);
extern void draw_string_set_tab_stops(const int16_t *tab_stops, int16_t count);

void game_engine_rasterize_message(const wchar_t *string, float alpha)
{
    /* DEVIATION: decompiler widens float alpha to double; DB proto uses float */
    int tag_index = interface_get_tag_index(_interface_font_terminal);
    /* {y0=70, x0=8, y1=94, x1=632} packed as 0x460008005E0278LL on PPC */
    const rectangle2d rect = {{70, 8, 94, 632}};
    int font_tag_index = interface_get_tag_index(_interface_font_terminal);
    draw_string_set_draw_mode(font_tag_index, -1, _text_justification_left, 0, global_real_argb_white);
    real_argb_color color;
    color.n[0] = alpha;
    color.n[1] = 0.45882353f;
    color.n[2] = 0.72941178f;
    color.n[3] = 1.0f;
    draw_string_set_draw_mode(tag_index, -1, _text_justification_center, (1u << _draw_text_bottom_justify_bit), &color);
    draw_string_set_color(&color);
    rasterizer_draw_unicode_string(&rect, 0, 0, 0, string);
    draw_string_set_format(-1, 0, 0);
    draw_string_set_tab_stops(0, 0);
}
