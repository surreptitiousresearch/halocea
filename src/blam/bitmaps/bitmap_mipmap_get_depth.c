/* bitmap_mipmap_get_depth @0x836F4978 */
#include <stdint.h>
#include "headers/bitmap_data.h"

int16_t bitmap_mipmap_get_depth(const bitmap_data *bitmap, int16_t mipmap_index)
{
    int v = (int)bitmap->depth >> mipmap_index;
    if (v <= 1)
        v = 1;
    return (int16_t)v;
}
