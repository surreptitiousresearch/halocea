#include "headers/bitmap_format_tables.h"

_D3DFORMAT rasterizer_bitmap_d3d_format(int fmt)
{
    return rasterizer_bitmap_format_table[fmt];
}
