#include "headers/bitmap_data.h"

short bitmap_mipmap_get_depth(const bitmap_data *bitmap, short mipmap_index)
{
    int v = (int)bitmap->depth >> mipmap_index;
    if (v <= 1)
        v = 1;
    return (short)v;
}
