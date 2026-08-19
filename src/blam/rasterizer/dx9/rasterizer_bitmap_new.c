/* rasterizer_bitmap_new @ 0x837934D8
   Creates the hardware texture backing a bitmap. If there is no device or the
   format has no hardware mapping, the bitmap gets no hardware format. The text
   cache bitmap is created unnamed; otherwise the bitmap's owning tag is named
   and its index within the tag's bitmap block is located so hcex_tex_new can
   register it.

   The per-tag bitmap block is the bitmap_group tag's `bitmaps` tag_block
   (offset 0x60, DB-verified); its elements are 48-byte bitmap_data records. */

#include <stdint.h>
#include "headers/bitmap_data.h"
#include "headers/bitmap_format_tables.h"
#include "headers/bitmap_group.h"
#include "headers/global_tag_instances.h"

extern bitmap_data *hardware_character_cache_get_bitmap(void);
extern char *tag_get_name(int tag_index);
extern void *hcex_tex_new(const char *name, int index_within_tag);

/* device presence is checked via the rasterizer engine global */
#include "headers/rasterizer_engine_globals.h"

uint8_t rasterizer_bitmap_new(bitmap_data *bitmap)
{
    /* disasm cmpwi r6,-1: the format table's "no hardware mapping" sentinel is -1,
       NOT d3d_render_boundary.h's D3DFMT_UNKNOWN (which is (_D3DFORMAT)0 in this TU).
       Keep the raw -1 sentinel (locked convention) to stay binary-faithful. */
    if ( !global_d3d_device
      || rasterizer_bitmap_format_table[bitmap->format] == (_D3DFORMAT)-1 )
    {
        bitmap->hardware_format = 0;
        return 1;
    }

    const char *name;
    int index_within_tag;

    if ( bitmap == hardware_character_cache_get_bitmap() )
    {
        index_within_tag = 0;
        name = 0;
    }
    else
    {
        name = tag_get_name(bitmap->tag_index);
        index_within_tag = 0;

        /* the tag instance's data block holds the bitmap group.
           DEVIATION: bitmap_data.tag_index (0x20, 32-bit) is a full tag datum handle with salt in
           the high word. @0x8379353C `clrlslwi r10, r10, 16,5` clears the high 16 bits before the
           <<5 (32-byte cache_file_tag_instance) stride, so the subscript is (uint16_t)tag_index —
           i.e. TAG_GET. The recovered source indexed with the whole 32-bit handle. */
        bitmap_group *group = TAG_GET(bitmap_group, bitmap->tag_index);
        if ( group->bitmaps.count > 0 )
        {
            bitmap_data *elements = (bitmap_data *)group->bitmaps.address;
            while ( index_within_tag < group->bitmaps.count )
            {
                if ( &elements[index_within_tag] == bitmap )
                    break;
                ++index_within_tag;
            }
        }
    }

    bitmap->hardware_format = hcex_tex_new(name, index_within_tag);
    return 1;
}
