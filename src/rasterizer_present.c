/* rasterizer_present @0x836A5DB0 — thunk to _rasterizer_present (present the frame, optional screenshot). */
#include "headers/point2d.h"
typedef struct bitmap_data bitmap_data;
extern void _rasterizer_present(bitmap_data *screenshot_bitmap, const point2d *screenshot_index);
void rasterizer_present(bitmap_data *screenshot_bitmap, const point2d *screenshot_index)
{
    _rasterizer_present(screenshot_bitmap, screenshot_index);
}
