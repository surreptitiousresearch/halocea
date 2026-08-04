/* rasterizer_bitmap_2d_get_pixel @0x836A6410 — thunk to the rasterizer's 2D bitmap sampler. */

#include "headers/bitmap_data.h"
#include "headers/real_point2d.h"

extern unsigned int _rasterizer_bitmap_2d_get_pixel(const bitmap_data *bitmap, const real_point2d *point, float lod);

unsigned int rasterizer_bitmap_2d_get_pixel(const bitmap_data *bitmap, const real_point2d *point,
        float lod)
{
    return _rasterizer_bitmap_2d_get_pixel(bitmap, point, lod);
}
