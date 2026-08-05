/* rasterizer_frame_statistics_draw @0x83790D10 — per-frame debug overlay for the rasterizer's frame timing.
 * Polls two hotkeys (F11 toggles the per-frame info readout, F12 toggles the framerate graph), feeds the
 * instantaneous fps into the framerate graph, renders the graph and the net graph, and — when fps accumulation
 * is enabled — draws a small two-row text table (header + values) of the windowed framerate statistics.
 *
 * DEVIATIONS (reconstructed against the disassembly; the decompiler mismodeled several constant-pool and
 * variadic-float accesses):
 *   - The decompiler rendered the data text color's first component as
 *     `*(float *)(HIDWORD(rasterizer_fps_accumulation_frame_index) + 11056)`. That is a misread of the constant
 *     1.0 load (the frame-index `ld` is a separate value used for the frame delta). The colors are plain literals.
 *   - The second sprintf's arguments come through as `COERCE_UNSIGNED_INT64(...)` halves: those are the float
 *     statistics promoted to double for the variadic call. Restored as the float fields they are.
 *   - All int->float conversions (instantaneous fps, frame delta, elapsed ms) are fcfid/frsp artifacts; restored
 *     as casts. */

#include <stdint.h>
#include "headers/rasterizer_frame_statistics_s.h"
#include "headers/rasterizer_globals.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/real_argb_color.h"
#include "headers/point2d.h"
#include "headers/rectangle2d.h"
#include "headers/key_code.h"
#include "headers/blam_data_globals.h"


extern uint8_t input_key_is_down(int16_t key_code);
extern uint32_t system_milliseconds(void);
extern void fg_init(void);
extern void fg_add_sample(int index, float sample);
extern void fg_render(uint8_t render_graph, uint8_t render_infos);
extern void net_graph_render(void);
extern rectangle2d *offset_rectangle2d(rectangle2d *rectangle, int16_t dx, int16_t dy);
extern void interface_set_bitmap_text_draw_mode(int16_t interface_font_index, int16_t style, int16_t justification, unsigned int flags, int16_t color_table_index, int16_t color_index);
extern void draw_string_set_tab_stops(const int16_t *tab_stops, int16_t count);
extern void draw_string_set_color(const real_argb_color *color);
extern void rasterizer_draw_string(const rectangle2d *bounds, const rectangle2d *clip, point2d *cursor_reference, int16_t height_adjust, const char *string);
extern int sprintf_0(char *string, const char *format, ...);

void rasterizer_frame_statistics_draw(void)
{
    /* Control + F11 toggles the info readout */
    if (input_key_is_down(_key_control) && input_key_is_down(_key_f11))
    {
        if (!debounce_f11)
        {
            debounce_f11 = 1;
            render_infos = 1 - render_infos;
        }
    }
    else
    {
        debounce_f11 = 0;
    }

    /* Control + F12 toggles the framerate graph */
    if (input_key_is_down(_key_control) && input_key_is_down(_key_f12))
    {
        if (!debounce_f12)
        {
            debounce_f12 = 1;
            render_graph = 1 - render_graph;
        }
    }
    else
    {
        debounce_f12 = 0;
    }

    /* instantaneous fps from the interval since the previous call */
    unsigned int now = system_milliseconds();
    unsigned int instantaneous_fps = now - oldtime;
    oldtime = now;
    if (instantaneous_fps != 0)
        instantaneous_fps = 1000 / instantaneous_fps;

    fg_init();
    fg_add_sample(0, (float)instantaneous_fps);
    fg_render(render_graph, render_infos);
    net_graph_render();

    if (rasterizer_debug_options.fps_accumulation)
    {
        point2d cursor[2];
        cursor[0].__s1.x = 0;
        cursor[0].__s1.y = 0;

        int16_t tab_stops[8];
        tab_stops[0] = 100;
        tab_stops[1] = 200;
        tab_stops[2] = 300;
        tab_stops[3] = 400;
        tab_stops[4] = 500;
        tab_stops[5] = 600;

        real_argb_color data_color;
        data_color.n[0] = 1.0f;
        data_color.n[1] = 0.66f;
        data_color.n[2] = 1.0f;
        data_color.n[3] = 0.66f;

        real_argb_color header_color;
        header_color.n[0] = 1.0f;
        header_color.n[1] = 1.0f;
        header_color.n[2] = 1.0f;
        header_color.n[3] = 1.0f;

        real_argb_color reset_color;
        reset_color.n[0] = 1.0f;
        reset_color.n[1] = 1.0f;
        reset_color.n[2] = 1.0f;
        reset_color.n[3] = 1.0f;

        int64_t frame_delta = rasterizer_globals.frame_index - rasterizer_fps_accumulation_frame_index;
        int16_t left_edge = rasterizer_globals.frame_bounds.n[1];
        int64_t elapsed_ms = system_milliseconds() - rasterizer_fps_accumulation_time;

        /* shift every column to the window's left edge */
        for (int i = 0; i < 6; i = (int16_t)(i + 1))
            tab_stops[i] += left_edge;

        rectangle2d bounds = rasterizer_globals.frame_bounds;
        offset_rectangle2d(&bounds, 0, 32);

        interface_set_bitmap_text_draw_mode(1, -1, 0, 0, 5, 0);

        char text[48];
        sprintf_0(text, "|n|tframerate|taverage (of %d)|tmin|tmax|tdropped",
                  rasterizer_frame_statistics.fps_sample_count);
        tab_stops[0] = left_edge;
        draw_string_set_tab_stops(tab_stops, 6);
        draw_string_set_color(&header_color);
        rasterizer_draw_string(&bounds, nullptr, cursor, -4, text);

        /* the values row sits one line below the header (cursor advanced by the draw above) */
        bounds.n[0] = cursor[0].__s1.y - 1;

        float window_average_fps = ((float)frame_delta * 1000.0f) / (float)elapsed_ms;
        sprintf_0(text, "|t%.0f|t%.0f/%.0f|t%.0f|t%.0f|t%5.1f%%|n",
                  rasterizer_frame_statistics.fps,
                  rasterizer_frame_statistics.fps_average,
                  window_average_fps,
                  rasterizer_frame_statistics.fps_min,
                  rasterizer_frame_statistics.fps_max,
                  rasterizer_frame_statistics.frames_dropped_pct);
        tab_stops[0] = left_edge;
        draw_string_set_tab_stops(tab_stops, 6);
        draw_string_set_color(&data_color);
        rasterizer_draw_string(&bounds, nullptr, cursor, -4, text);

        draw_string_set_tab_stops(nullptr, 0);
        draw_string_set_color(&reset_color);
    }
}
