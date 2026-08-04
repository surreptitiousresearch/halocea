/* rasterizer_xbox_bitmap_get_pixel_data_size @0x837E3CC8 — tail-call forwarder
 * (`b rasterizer_bitmap_get_hw_size`; confirmed via disasm, not a bare blr). rasterizer_bitmap_get_hw_
 * size itself is not yet decompiled — declared extern with its own DB prototype and called through
 * with the same argument. */

#include "headers/bitmap_data.h"

extern int rasterizer_bitmap_get_hw_size(const bitmap_data *bitmap);

int rasterizer_xbox_bitmap_get_pixel_data_size(const bitmap_data *bitmap)
{
    return rasterizer_bitmap_get_hw_size(bitmap);
}
