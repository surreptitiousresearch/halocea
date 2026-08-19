/* bitmap_3d_vector_map @0x8377C6D0 — convert a 3D (volume) bitmap in place into a normalized vector map (source:
 * bitmap_utilities.c). For each depth slice it extracts the slice to a scratch 2D bitmap and, for every texel,
 * decodes the three color bytes to a [-1,1] vector (c/127.5 - 1), normalizes it (leaving near-zero vectors
 * unchanged), re-encodes each component to [0,255] via (c+1)*127.5 + 0.5, preserves the original alpha, and
 * writes the result back; the slice is reinserted into the volume. Sibling of bitmap_cm_vector_map (cube-map
 * variant) and bitmap_3d_alpha_bleed (same per-slice scaffolding).
 *
 * DEVIATION: Hex-Rays reported "local variable allocation has failed" and punned the channel bytes through
 * OVERLAPPED __int64s. Disasm (0x8377C7D4-0x8377C8C0) settles the mapping: the low/high bytes (byte0, bits 0-7,
 * and byte2, bits 16-23) map straight through, but the two middle color bytes are SWAPPED on write —
 * output = (encode(norm(byte1)) << 16) | (encode(norm(byte2)) << 8) | encode(norm(byte0)) | alpha. This is unlike
 * bitmap_cm_vector_map, which writes the channels back in place with no swap; the swap here is real, not a
 * decompiler artifact. */

#include <stdint.h>
#include "headers/bitmap_data.h"
#include <string.h>
#include "headers/ppc_intrinsics.h"

extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);
extern void dlFree(void *ptr);

extern bitmap_data *bitmap_2d_new(int16_t width, int16_t height, int16_t mipmap_count, int16_t format);
extern void bitmap_3d_slice_extract(const bitmap_data *source_bitmap, int16_t source_mipmap_index, int16_t source_slice_index, bitmap_data *slice_bitmap);
extern int bitmap_get_pixel_data_size(const bitmap_data *bitmap);
extern char *bitmap_2d_address(const bitmap_data *bitmap, int16_t x, int16_t y, int16_t mipmap_index);
extern void * bitmap_mipmap_address(const bitmap_data *bitmap, int16_t mipmap_index);
extern void bitmap_3d_slice_insert(const bitmap_data *slice_bitmap, bitmap_data *destination_bitmap, int16_t destination_mipmap_index, int16_t destination_slice_index);
extern void bitmap_delete(bitmap_data *bitmap);

void bitmap_3d_vector_map(bitmap_data *bitmap)
{
    bitmap_data *slice = bitmap_2d_new(bitmap->width, bitmap->height, 0, bitmap->format);
    if ( slice && slice->base_address )
    {
        for ( int z = 0; z < bitmap->depth; ++z )
        {
            bitmap_3d_slice_extract(bitmap, 0, z, slice);
            unsigned int pixel_data_size = bitmap_get_pixel_data_size(slice);
            char *buffer = dlMalloc(pixel_data_size,
                    "D:\\Projects\\code\\HCEX\\sources\\bitmaps\\bitmap_utilities.c", 0x587);
            if ( buffer )
            {
                for ( int y = 0; y < slice->height; ++y )
                {
                    for ( int x = 0; x < slice->width; ++x )
                    {
                        unsigned int texel = *(unsigned int *)bitmap_2d_address(slice, x, y, 0);
                        float c0 = (float)(texel & 0xFF) * 0.0078431377f - 1.0f;
                        float c1 = (float)((texel >> 8) & 0xFF) * 0.0078431377f - 1.0f;
                        float c2 = (float)((texel >> 16) & 0xFF) * 0.0078431377f - 1.0f;

                        float magnitude = __fsqrts(c1 * c1 + (c0 * c0 + c2 * c2));
                        if ( __fabs(magnitude) >= 0.000099999997f )
                        {
                            float inverse = 1.0f / magnitude;
                            c1 *= inverse;
                            c2 *= inverse;
                            c0 *= inverse;
                        }

                        unsigned int e0 = (int)((c0 + 1.0f) * 127.5f + 0.5f);
                        unsigned int e1 = (int)((c1 + 1.0f) * 127.5f + 0.5f);
                        unsigned int e2 = (int)((c2 + 1.0f) * 127.5f + 0.5f);

                        *(unsigned int *)&buffer[4 * (slice->width * y + x)] =
                                (e1 << 16) | (e2 << 8) | e0 | (texel & 0xFF000000);
                    }
                }

                memcpy(bitmap_mipmap_address(slice, 0), buffer, pixel_data_size);
                dlFree(buffer);
            }
            bitmap_3d_slice_insert(slice, bitmap, 0, z);
        }
    }
    bitmap_delete(slice);
}
