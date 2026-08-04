/* targa_export @0x83767758 — write a bitmap out as an uncompressed 32-bit top-down TGA file. Returns NULL on
 * success or a static error string describing the failure stage. */

typedef struct file_reference file_reference;
#include <stdint.h>
#include "headers/bitmap_data.h"

extern uint8_t file_create(file_reference *file);
extern uint8_t file_open(file_reference *file, unsigned int flags);
extern uint8_t file_write(const file_reference *file, unsigned int count, const void *buffer);
extern uint8_t file_close(file_reference *file);
extern char *bitmap_2d_address(const bitmap_data *bitmap, int16_t x, int16_t y, int16_t mipmap_index);

#include "headers/targa_header.h"

const char *targa_export(file_reference *file, const bitmap_data *bitmap)
{
    const char *error = 0;

    if ( !file_create(file) || !file_open(file, 2u) )
        return "couldn't open file";

    targa_header header;
    header.id_length = 0;
    header.colormap_type = 0;
    header.image_type = 2;
    /* recovered: _colormap_pad + colormap_spec -> colormap_specification[5] */
    for ( int i = 0; i < 5; ++i )
        header.colormap_specification[i] = 0;
    header.x_origin = 0;
    header.y_origin = 0;
    header.image_width = bitmap->width;   /* recovered: width -> image_width */
    header.image_height = bitmap->height; /* recovered: height -> image_height */
    header.pixel_depth = 32;              /* recovered: bits_per_pixel -> pixel_depth */
    header.image_descriptor = 40;         /* recovered: descriptor -> image_descriptor */

    if ( !file_write(file, 0x12u, &header) )
    {
        error = "couldn't write header";
        file_close(file);
        return error;
    }

    unsigned int row_bytes = 4 * bitmap->width;
    for ( int row = 0; row < bitmap->height; ++row )
    {
        const void *pixels = bitmap_2d_address(bitmap, 0, row, 0);
        if ( !file_write(file, row_bytes, pixels) )
        {
            file_close(file);
            return "couldn't write row";
        }
    }

    file_close(file);
    return 0;
}
