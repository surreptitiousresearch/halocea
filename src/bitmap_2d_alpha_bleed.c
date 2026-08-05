/* bitmap_2d_alpha_bleed @0x8377BBA0 — bleed opaque color into fully transparent texels so bilinear filtering
 * does not sample garbage RGB at alpha edges. For each requested pass it copies the whole 2D bitmap into a
 * scratch buffer: every texel whose alpha is zero is replaced by the RGB of the first opaque texel found in its
 * 3x3 neighborhood (alpha left at zero), while opaque texels are copied unchanged. Each pass writes the scratch
 * result back to the bitmap, so successive passes bleed color outward one ring at a time. */

#include <stdint.h>
#include "headers/bitmap_data.h"

extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);
extern void dlFree(void *ptr);
extern void *memcpy(void *dest, const void *src, unsigned int count);
extern int bitmap_get_pixel_data_size(const bitmap_data *bitmap);
extern char *bitmap_2d_address(const bitmap_data *bitmap, int16_t x, int16_t y, int16_t mipmap_index);
extern void * bitmap_mipmap_address(const bitmap_data *bitmap, int16_t mipmap_index);

void bitmap_2d_alpha_bleed(bitmap_data *bitmap, int16_t passes)
{
    unsigned int pixel_data_size = bitmap_get_pixel_data_size(bitmap);
    char *scratch = dlMalloc(
        pixel_data_size, "D:\\Projects\\code\\HCEX\\sources\\bitmaps\\bitmap_utilities.c", 0x422u);
    if ( !scratch )
        return;

    int pass_count = passes;
    if ( passes > 0 )
    {
        int pass = 0;
        do
        {
            if ( bitmap->height > 0 )
            {
                int y = 0;
                do
                {
                    unsigned int *source_row = (unsigned int *)bitmap_2d_address(bitmap, 0, y, 0);
                    int width = bitmap->width;
                    char *dest_row = &scratch[4 * width * y];
                    if ( width > 0 )
                    {
                        int x = 0;
                        do
                        {
                            int pixel = source_row[x];
                            if ( (pixel & 0xFF000000) == 0 )
                            {
                                uint8_t found = 0;
                                for ( int16_t dy = -1; dy <= 1 && !found; ++dy )
                                {
                                    for ( int16_t dx = -1; dx <= 1 && !found; ++dx )
                                    {
                                        int nx = (int16_t)(dx + x);
                                        int ny = dy + y;
                                        if ( nx >= 0 && ny >= 0 && nx < bitmap->width && ny < bitmap->height )
                                        {
                                            int neighbor = *(int *)bitmap_2d_address(bitmap, nx, ny, 0);
                                            if ( neighbor )
                                            {
                                                pixel = neighbor & 0xFFFFFF;
                                                found = 1;
                                            }
                                        }
                                    }
                                }
                            }
                            *(int *)&dest_row[4 * x] = pixel;
                            x = (int16_t)(x + 1);
                        }
                        while ( x < bitmap->width );
                    }
                    y = (int16_t)(y + 1);
                }
                while ( y < bitmap->height );
            }
            memcpy(bitmap_mipmap_address(bitmap, 0), scratch, pixel_data_size);
            pass = (int16_t)(pass + 1);
        }
        while ( pass < pass_count );
    }

    dlFree(scratch);
}
