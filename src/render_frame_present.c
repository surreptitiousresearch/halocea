/* render_frame_present @0x83707718 — present the frame's bitmap via the rasterizer, optionally capturing a
 * screenshot at the given index. */
#include "headers/point2d.h"
typedef struct bitmap_data bitmap_data;
extern void rasterizer_present(bitmap_data *screenshot_bitmap, const point2d *screenshot_index);
void render_frame_present(const point2d *screenshot_index, bitmap_data *bitmap)
{
    rasterizer_present(bitmap, screenshot_index);
}
