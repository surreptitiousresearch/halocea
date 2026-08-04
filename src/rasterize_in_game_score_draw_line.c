/* rasterize_in_game_score_draw_line @0x837496C0 — draws one row of the in-game scoreboard: picks a
 * tab-stop table by `tab_stop_index` (skipped in favor of a narrow default when the viewport is <=320px
 * wide), sets up the font draw mode (brightening `color` by 0.4 per channel when `hilite`), computes the
 * row's vertical placement from the interface font's line-height metrics (a compact ascending+leading
 * height when running split-screen, the full ascending+descending+leading height otherwise), then draws
 * `text`. `line == 0` is the title row (no tab stops, no visible text draw skip — just falls through to
 * the same row-0 vertical slot as everything else). */

#include <wchar.h>
#include <stdint.h>
#include "headers/real_argb_color.h"
#include "headers/render_globals.h"
#include "headers/global_tag_instances.h"
#include "headers/font_header.h"
#include "headers/point2d.h"
#include "headers/interface_tag_index.h"
#include "headers/text_justification.h"
#include "headers/blam_data_globals.h"


#include "headers/point2d.h"
extern int16_t local_player_count(void);
extern int interface_get_tag_index(int16_t interface_tag_index);
extern rectangle2d *offset_rectangle2d(rectangle2d *rectangle, int16_t dx, int16_t dy);
extern void draw_string_set_tab_stops(const int16_t *tab_stops, int16_t count);
extern void draw_string_set_draw_mode(int font_index, int16_t style, int16_t justification, unsigned int flags, const real_argb_color *color);
extern void rasterizer_draw_unicode_string(const rectangle2d *bounds, const rectangle2d *clip, point2d *cursor_reference, int16_t height_adjust, const wchar_t *string);

void rasterize_in_game_score_draw_line(const wchar_t *text, uint8_t hilite, real_argb_color *_color, int line, int tab_stop_index)
{
    real_argb_color color = *_color;
    rectangle2d window_bounds = render.camera.window_bounds;

    unsigned __int8 split_screen = local_player_count() > 1;
    int tag_index = interface_get_tag_index(_interface_font_terminal);

    static const __int16 title_tab_stops[3] = {25, 90, 300};
    static const __int16 header_tab_stops[3] = {300, 365, 515};
    static const __int16 wide_tab_stops[3] = {130, 195, 315};
    static const __int16 default_tab_stops[3] = {80, 125, 200};

    /* rectangle2d is a union; named edges live under the __s1 view */
    unsigned __int8 viewport_wide_enough = (window_bounds.__s1.x1 - window_bounds.__s1.x0) > 0x140;
    const __int16 *tab_stops;

    if (!tab_stop_index)
        tab_stops = title_tab_stops;
    else if (!viewport_wide_enough)
        tab_stops = default_tab_stops;
    else if (tab_stop_index == 1)
        tab_stops = header_tab_stops;
    else if (tab_stop_index == 2)
        tab_stops = wide_tab_stops;
    else
        tab_stops = default_tab_stops;

    __int16 tab_stop_count = line ? 3 : 0;
    if (!line)
        tab_stops = NULL;

    draw_string_set_tab_stops(tab_stops, tab_stop_count);

    offset_rectangle2d(&window_bounds, -render.camera.viewport_bounds.__s1.x0, -render.camera.viewport_bounds.__s1.y0);

    if (tag_index != -1)
    {
        font_header *font = TAG_GET(font_header, tag_index);
        __int16 line_height = split_screen ? (font->leading_height + font->ascending_height)
                : (font->leading_height + font->descending_height + font->ascending_height);

        if (hilite)
        {
            color.n[1] += 0.4f;
            color.n[2] += 0.4f;
            color.n[3] += 0.4f;
            if (color.n[1] > 1.0f)
                color.n[1] = 1.0f;
            if (color.n[2] > 1.0f)
                color.n[2] = 1.0f;
            if (color.n[3] > 1.0f)
                color.n[3] = 1.0f;
        }

        window_bounds.__s1.y0 = (short)((line + 4) * line_height);
        window_bounds.__s1.y1 = (short)(window_bounds.__s1.y0 + line_height);
        draw_string_set_draw_mode(tag_index, -1, _text_justification_left, 0, &color);
        rasterizer_draw_unicode_string(&window_bounds, NULL, NULL, 0, text);
    }

    draw_string_set_tab_stops(NULL, 0);
}
