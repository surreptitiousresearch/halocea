#include <stdint.h>
#include "headers/bitmap_data.h"

extern char *bitmap_2d_address(const bitmap_data *bitmap, int16_t x, int16_t y, int16_t mipmap_index);
extern int bitmap_get_pixel_count(const bitmap_data *bitmap);

void bitmap_fill(bitmap_data *bitmap, unsigned int color)
{
    unsigned int *pixel = (unsigned int *)bitmap_2d_address(bitmap, 0, 0, 0);
    int pixel_count = bitmap_get_pixel_count(bitmap);

    for (int i = 0; i < pixel_count; i++)
        pixel[i] = color;
}
