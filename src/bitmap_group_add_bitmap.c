/* bitmap_group_add_bitmap @0x83794210 — append a new, blank bitmap of the given dimensions/type/format to a
 * bitmap group's bitmaps block (growing its pixel-data blob accordingly) and return the new bitmap's index (or
 * -1 on rejection/allocation failure). It builds a bitmap_data descriptor ('bitm' signature), then validates the
 * request: interface-bitmap groups (bitmap_group_type_interface_bitmaps) are always accepted (linear bit 0x10);
 * otherwise all dimensions must be powers of two (pow2 bit 0x1) and cube maps must have square faces — a failure
 * logs a diagnostic to stderr and rejects. Compressed formats (dxt1..dxt5) set the compressed bit 0x2, p8_bump
 * sets the palettized bit 0x4. On acceptance it grows the
 * bitmaps block and the pixel_data blob, re-points every existing bitmap's base_address into the (possibly
 * moved) pixel_data, computes the new bitmap's pixel offset from the running total, copies the descriptor in,
 * wires its offset/base_address, and zero-fills its pixel region.
 *
 * DEVIATION: bitmap_data is 48 bytes; the tail's `_DWORD *v31` writes are word offsets 6/11 = byte +24
 * (pixels_offset) / +44 (base_address). The diagnostic fprintf/fflush go to `__iob_func() + 1` (stderr) as
 * emitted. */

#include <stdint.h>
#include "headers/bitmap_group.h"
#include "headers/bitmap_group_type.h"
#include "headers/bitmap_data.h"
#include "headers/bitmap_format.h"
#include "headers/bitmap_flags.h"
#include "headers/tag_block.h"
#include "headers/tag_data.h"
#include <stdio.h>
#include <string.h>

typedef struct _iobuf _iobuf;
extern _iobuf *__iob_func(void);
extern int fprintf(_iobuf *stream, const char *format, ...);
extern int fflush(_iobuf *stream);
extern int bitmap_get_pixel_data_size(const bitmap_data *bitmap);
extern uint8_t tag_block_resize(tag_block *block, int element_count);
extern uint8_t tag_data_resize(tag_data *data, int size);

int16_t bitmap_group_add_bitmap(bitmap_group *group, int16_t width, int16_t height, int16_t depth, int16_t type, int16_t format, int16_t mipmap_count)
{
    int16_t group_type = group->type;

    bitmap_data new_bitmap;
    new_bitmap.width = width;
    new_bitmap.height = height;
    new_bitmap.depth = depth;
    new_bitmap.signature = 0x6269746Du;   /* 'bitm' */
    new_bitmap.type = type;
    new_bitmap.format = format;
    new_bitmap.flags = 0;
    new_bitmap.registration_point.x = 0;
    new_bitmap.registration_point.y = 0;
    new_bitmap.mipmap_count = mipmap_count;
    new_bitmap.pixels_offset = 0;
    new_bitmap.hardware_format = nullptr;
    new_bitmap.base_address = nullptr;

    uint16_t flags;
    uint8_t accepted = 1;
    if ( group_type == _bitmap_group_type_interface_bitmaps )
    {
        flags = (1u << _bitmap_linear_bit);   /* 0x10 */
        new_bitmap.flags = flags;
    }
    else
    {
        if ( ((width - 1) & width) != 0 || ((height - 1) & height) != 0 || ((depth - 1) & depth) != 0 )
        {
            fprintf(__iob_func() + 1, "skipping bitmap with non-power-of-two dimensions (#%dx#%d#%d)\r\n",
                    width, height, depth);
        }
        else if ( group_type != _bitmap_group_type_cube_maps || width == height )
        {
            flags = (1u << _bitmap_has_power_of_two_dimensions_bit);   /* 0x1 */
            new_bitmap.flags = flags;
            goto validated;
        }
        else
        {
            fprintf(__iob_func() + 1, "skipping cube map with non-square faces (#%dx#%d)\r\n", width, height);
        }
        fflush(__iob_func() + 1);
        flags = new_bitmap.flags;
        accepted = 0;
    }
validated:
    if ( !accepted )
        return -1;

    if ( format >= _bitmap_format_dxt1 && format <= _bitmap_format_dxt5 )
    {
        flags |= (1u << _bitmap_compressed_bit);   /* 0x2 */
        new_bitmap.flags = flags;
    }
    if ( format == _bitmap_format_p8_bump )
    {
        flags |= (1u << _bitmap_palettized_bit);   /* 0x4 */
        new_bitmap.flags = flags;
    }

    if ( group->type == _bitmap_group_type_cube_maps && width != height )
    {
        fprintf(__iob_func() + 1, "skipping cube map with non-square faces (#%dx#%d)\r\n", width, height);
        fflush(__iob_func() + 1);
        return -1;
    }
    if ( (flags & (1u << _bitmap_has_power_of_two_dimensions_bit)) == 0
      && group->type != _bitmap_group_type_interface_bitmaps )
    {
        fprintf(__iob_func() + 1, "skipping bitmap with non power-of-two dimensions (#%dx#%d)\r\n", width, height);
        fflush(__iob_func() + 1);
        return -1;
    }

    int previous_count = group->bitmaps.count;
    int pixel_data_size = bitmap_get_pixel_data_size(&new_bitmap);
    if ( !tag_block_resize(&group->bitmaps, group->bitmaps.count + 1)
      || !tag_data_resize(&group->pixel_data, group->pixel_data.size + pixel_data_size) )
    {
        tag_block_resize(&group->bitmaps, previous_count);
        return -1;
    }

    /* re-point every existing bitmap into the (possibly relocated) pixel_data, and find the running end offset */
    int pixels_end = 0;
    const bitmap_data *last_with_pixels = nullptr;
    for ( int i = 0; i < group->bitmaps.count; i = (int16_t)(i + 1) )
    {
        bitmap_data *existing = &((bitmap_data *)group->bitmaps.address)[i];
        if ( existing->base_address )
        {
            existing->base_address = (char *)group->pixel_data.address + existing->pixels_offset;
            last_with_pixels = existing;
            pixels_end = bitmap_get_pixel_data_size(existing) + existing->pixels_offset;
        }
    }
    (void)last_with_pixels;

    bitmap_data *slot = &((bitmap_data *)group->bitmaps.address)[(int16_t)previous_count];
    memcpy(slot, &new_bitmap, 0x30u);
    slot->pixels_offset = pixels_end;
    char *pixels = (char *)group->pixel_data.address + pixels_end;
    slot->base_address = pixels;
    memset(pixels, 0, pixel_data_size);

    return (int16_t)previous_count;
}
