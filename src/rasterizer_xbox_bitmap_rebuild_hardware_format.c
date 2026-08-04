/* rasterizer_xbox_bitmap_rebuild_hardware_format @0x837E3CD0 — repack a bitmap's mip/face pixel data into the
 * Xbox hardware texture layout in a scratch buffer, then copy it back over the bitmap's pixels. Iterates each
 * cube face (6 for cubemaps, else 1) and each mip level: for hardware-aligned bitmaps (flag 0x10) it copies row
 * by row, zero-padding each row up to a 64-byte pitch; otherwise it copies the whole face slice (remapped
 * through face_mapping_inverse_table so cube faces land in hardware face order). Each face is then zero-padded
 * up to a 128-byte boundary. Returns 1 (0 if the scratch allocation fails). */

#include <stdint.h>
#include "headers/bitmap_data.h"
#include "headers/bitmap_type.h"
#include "headers/bitmap_flags.h"

extern const int face_mapping_inverse_table[];

extern int rasterizer_bitmap_get_hw_size(const bitmap_data *bitmap);
extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);
extern void dlFree(void *ptr);
extern int16_t rasterizer_xbox_bitmap_get_max_mipmap_count(const bitmap_data *bitmap);
extern void * bitmap_mipmap_address(const bitmap_data *bitmap, int16_t mipmap_index);
extern int bitmap_mipmap_get_pixel_data_size(const bitmap_data *bitmap, int16_t mipmap_index);
extern int bitmap_mipmap_get_row_pitch(const bitmap_data *bitmap, int16_t mipmap_index);
extern void *memcpy(void *dest, const void *src, unsigned int count);
extern void *memset(void *dest, int value, unsigned int count);

int rasterizer_xbox_bitmap_rebuild_hardware_format(bitmap_data *bitmap)
{
    unsigned int hardware_size = rasterizer_bitmap_get_hw_size(bitmap);
    unsigned int write_offset = 0;

    __int16 face_count = 6;
    if ( bitmap->type != bitmap_type_cube_map )
        face_count = 1;

    char *buffer = dlMalloc(
        hardware_size, "D:\\Projects\\code\\HCEX\\sources\\rasterizer\\rasterizer_swizzle.c", 0x230u);
    if ( !buffer )
        return 0;

    int faces = face_count;
    if ( face_count > 0 )
    {
        for ( int face = 0; face < faces; face = (__int16)(face + 1) )
        {
            int max_mipmap_count = rasterizer_xbox_bitmap_get_max_mipmap_count(bitmap);
            if ( max_mipmap_count >= 0 )
            {
                for ( __int16 mipmap = 0; mipmap <= max_mipmap_count; ++mipmap )
                {
                    char *source = bitmap_mipmap_address(bitmap, mipmap);
                    unsigned int slice_size = bitmap_mipmap_get_pixel_data_size(bitmap, mipmap);
                    if ( bitmap->type == bitmap_type_cube_map )
                        slice_size = slice_size / 6;

                    if ( (bitmap->flags & (1u << _bitmap_linear_bit)) != 0 )
                    {
                        unsigned int row_pitch = bitmap_mipmap_get_row_pitch(bitmap, mipmap);
                        unsigned int row_padding = -row_pitch & 0x3F;
                        if ( bitmap->height > 0 )
                        {
                            int row = 0;
                            do
                            {
                                memcpy(&buffer[write_offset], source, row_pitch);
                                unsigned int padded_offset = row_pitch + write_offset;
                                memset(&buffer[padded_offset], 0, row_padding);
                                source += row_pitch;
                                write_offset = row_padding + padded_offset;
                                row = (__int16)(row + 1);
                            }
                            while ( row < bitmap->height );
                        }
                    }
                    else
                    {
                        memcpy(&buffer[write_offset], &source[face_mapping_inverse_table[face] * slice_size], slice_size);
                        write_offset += slice_size;
                    }
                }
            }
            memset(&buffer[write_offset], 0, -write_offset & 0x7F);
            write_offset += -write_offset & 0x7F;
        }
    }

    memcpy(bitmap->base_address, buffer, hardware_size);
    dlFree(buffer);
    return 1;
}
