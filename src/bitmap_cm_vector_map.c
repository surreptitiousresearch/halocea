/* bitmap_cm_vector_map @0x8377C958 — convert a cube-map bitmap in place into a normalized "vector map" (source:
 * bitmap_utilities.c). For each of the 6 cube faces it extracts the face to a scratch 2D bitmap, then for every
 * texel decodes the R/G/B bytes to a [-1,1] vector (c/127.5 - 1), normalizes it (leaving near-zero vectors
 * unchanged), re-encodes each component to [0,255] via (c+1)*127.5 + 0.5, preserves the original alpha byte, and
 * writes the result back; the rebuilt face is inserted back into the cube map. The scratch bitmap is deleted at
 * the end.
 *
 * DEVIATION: Hex-Rays reported "local variable allocation has failed" and mislabeled the channel byte extraction
 * (its (u8)/BYTE1/BYTE2 splits do not match the actual extrwi bit positions). Disasm (0x8377CA4C-8377CB38)
 * settles it: red = (texel>>16)&0xFF, green = (texel>>8)&0xFF, blue = texel&0xFF, and the re-encoded bytes go
 * back to the same channels (no swap); alpha (texel & 0xFF000000) is carried through. */

#include <stdint.h>
#include "headers/bitmap_data.h"
#include <string.h>
extern float __fsqrts(float);

extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);
extern void dlFree(void *ptr);

extern bitmap_data *bitmap_2d_new(int16_t width, int16_t height, int16_t mipmap_count, int16_t format);
extern void bitmap_cube_map_face_extract(const bitmap_data *source_bitmap, int16_t source_mipmap_index, int16_t source_face_index, bitmap_data *face_bitmap);
extern int bitmap_get_pixel_data_size(const bitmap_data *bitmap);
extern char *bitmap_2d_address(const bitmap_data *bitmap, int16_t x, int16_t y, int16_t mipmap_index);
extern void * bitmap_mipmap_address(const bitmap_data *bitmap, int16_t mipmap_index);
extern void bitmap_cube_map_face_insert(const bitmap_data *face_bitmap, bitmap_data *destination_bitmap, int16_t destination_mipmap_index, int16_t destination_face_index);
extern void bitmap_delete(bitmap_data *bitmap);

void bitmap_cm_vector_map(bitmap_data *bitmap)
{
    bitmap_data *face = bitmap_2d_new(bitmap->width, bitmap->height, 0, bitmap->format);
    if ( face && face->base_address )
    {
        for ( int16_t face_index = 0; face_index < 6; ++face_index )
        {
            bitmap_cube_map_face_extract(bitmap, 0, face_index, face);
            unsigned int pixel_data_size = bitmap_get_pixel_data_size(face);
            char *buffer = dlMalloc(pixel_data_size,
                    "D:\\Projects\\code\\HCEX\\sources\\bitmaps\\bitmap_utilities.c", 0x587);
            if ( buffer )
            {
                for ( int y = 0; y < face->height; ++y )
                {
                    for ( int x = 0; x < face->width; ++x )
                    {
                        unsigned int texel = *(unsigned int *)bitmap_2d_address(face, x, y, 0);
                        float red   = (float)((texel >> 16) & 0xFF) * 0.0078431377f - 1.0f;
                        float green = (float)((texel >> 8) & 0xFF) * 0.0078431377f - 1.0f;
                        float blue  = (float)(texel & 0xFF) * 0.0078431377f - 1.0f;

                        float magnitude = __fsqrts(red * red + (green * green + blue * blue));
                        if ( __fabs(magnitude) >= 0.000099999997 )
                        {
                            float inverse = 1.0f / magnitude;
                            red   *= inverse;
                            green *= inverse;
                            blue  *= inverse;
                        }

                        unsigned int encoded_red   = (int)((red + 1.0f) * 127.5f + 0.5f);
                        unsigned int encoded_green = (int)((green + 1.0f) * 127.5f + 0.5f);
                        unsigned int encoded_blue  = (int)((blue + 1.0f) * 127.5f + 0.5f);

                        *(unsigned int *)&buffer[4 * (face->width * y + x)] =
                                (encoded_red << 16) | (encoded_green << 8) | encoded_blue | (texel & 0xFF000000);
                    }
                }

                memcpy(bitmap_mipmap_address(face, 0), buffer, pixel_data_size);
                dlFree(buffer);
            }

            bitmap_cube_map_face_insert(face, bitmap, 0, face_index);
        }
    }
    bitmap_delete(face);
}
