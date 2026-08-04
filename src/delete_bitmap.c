#include "headers/tag_block.h"
#include "headers/bitmap_data.h"

extern void bitmap_delete(bitmap_data *bitmap);

void delete_bitmap(tag_block *bitmaps_block, int bitmap_index)
{
    bitmap_delete((bitmap_data *)bitmaps_block->address + bitmap_index);
}
