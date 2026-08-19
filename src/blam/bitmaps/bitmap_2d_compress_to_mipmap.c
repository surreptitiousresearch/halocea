/* bitmap_2d_compress_to_mipmap @0x8377CBC8 — no-op (bare blr; confirmed via disasm). */

#include <stdint.h>
#include "headers/bitmap_data.h"

void bitmap_2d_compress_to_mipmap(const bitmap_data *source_bitmap, bitmap_data *destination_bitmap, int16_t destination_mipmap_index, const unsigned int *transparent_color)
{
}
