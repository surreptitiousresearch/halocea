/* rasterizer_bitmap_get_hw_size @0x83793468 — computes the hardware-texture byte size for a bitmap by
 * delegating to the `hcex` bridge layer's platform-specific size calculator. */

#include "headers/bitmap_data.h"
#include "headers/bitmap_format_tables.h"

/* hcex_* is the HCEX bridge layer boundary (see CLAUDE.md) — declared extern, not reversed. */
extern int hcex_calc_tex_hw_size(int width, int height, int mipmap_count, int depth, _D3DFORMAT hardware_format,
        int type);

int rasterizer_bitmap_get_hw_size(const bitmap_data *bitmap)
{
    return hcex_calc_tex_hw_size(bitmap->width, bitmap->height, bitmap->mipmap_count + 1, bitmap->depth,
            rasterizer_bitmap_format_table[bitmap->format], bitmap->type);
}
