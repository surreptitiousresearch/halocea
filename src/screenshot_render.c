/* screenshot_render @0x8368B680 — capture queued screenshots by rendering the scene into an off-screen
 * bitmap and writing TGA files. global_screenshot_size (clamped to 1..3) is a super-sampling factor and
 * global_screenshot_count is the page grid dimension, so an off-screen bitmap size*screen wide and tall is
 * allocated and the world is rendered page-by-page (count x count), each page super-sampled (size x size)
 * before being exported as screenshots\<id>screenshot<page_y><page_x>.tga. When only a single 1x page is
 * requested the simple (untiled) render path is used. The capture queue is cleared afterwards.
 *
 * DEVIATION: the decompiler carried the loop counters inside the point2d locals and threaded a redundant v6
 * copy of the page x index; restored to plain nested loops. */

#include <stdint.h>
#include "headers/render_window.h"
#include "headers/point2d.h"
#include "headers/bitmap_data.h"
#include "headers/file_reference.h"
#include "headers/main_globals.h"
#include "headers/rasterizer_globals.h"
#include "headers/bitmap_format.h"
#include "headers/blam_data_globals.h"


extern bitmap_data *bitmap_2d_new(int16_t width, int16_t height, int16_t mipmap_count, int16_t format);
extern void console_printf(uint8_t clear, const char *format, ...);
extern void console_close(void);
extern void render_frame(const struct render_window *windoze, int16_t window_count, const point2d *screenshot_page_index, const point2d *screenshot_index, bitmap_data *screenshot_bitmap, float time_delta_since_tick_sec, float time_delta_since_frame_sec);
extern void render_frame_present(const point2d *screenshot_index, bitmap_data *bitmap);
extern int sprintf_0(char *string, const char *format, ...);
extern uint8_t system_ensure_directory_tree_exists(const char *directory);
extern file_reference *file_reference_create_from_path(file_reference *reference, const char *path, uint8_t directory);
extern const char *targa_export(file_reference *file, const bitmap_data *bitmap);
extern void bitmap_delete(bitmap_data *bitmap);

void screenshot_render(struct render_window *window)
{
    int size = global_screenshot_size;
    if (size < 1)
        size = 1;
    else if (size > 3)
        size = 3;
    global_screenshot_size = size;

    __int16 width = size * (rasterizer_globals.screen_bounds.n[3] - rasterizer_globals.screen_bounds.n[1]);
    __int16 height = size * (rasterizer_globals.screen_bounds.n[2] - rasterizer_globals.screen_bounds.n[0]);
    bitmap_data *bitmap = bitmap_2d_new(width, height, 0, _bitmap_format_x8r8g8b8);

    if (!bitmap || !bitmap->base_address)
    {
        global_screenshot_count = 0;
        return;
    }

    console_printf(1, "");
    console_close();

    for (int page_y = 0; page_y < global_screenshot_count; ++page_y)
    {
        for (int page_x = 0; page_x < global_screenshot_count; ++page_x)
        {
            for (int tile_y = 0; tile_y < global_screenshot_size; ++tile_y)
            {
                for (int tile_x = 0; tile_x < global_screenshot_size; ++tile_x)
                {
                    point2d page_index = { (short)page_x, (short)page_y };
                    point2d tile_index = { (short)tile_x, (short)tile_y };
                    if (global_screenshot_count > 1 || global_screenshot_size > 1)
                    {
                        render_frame(window, 1, &page_index, &tile_index, bitmap, 0.0f, 0.0f);
                        render_frame_present(&tile_index, bitmap);
                    }
                    else
                    {
                        render_frame(window, 1, nullptr, nullptr, bitmap, 0.0f, 0.0f);
                        render_frame_present(nullptr, bitmap);
                    }
                }
            }

            char filename[512];
            sprintf_0(filename, "%s\\%dscreenshot%d%d.tga", "screenshots",
                      main_globals.screenshot_identifier, page_y, page_x);
            system_ensure_directory_tree_exists("screenshots");

            file_reference file;
            file_reference_create_from_path(&file, filename, 0);
            targa_export(&file, bitmap);
        }
    }

    ++main_globals.screenshot_identifier;
    bitmap_delete(bitmap);
    global_screenshot_count = 0;
}
