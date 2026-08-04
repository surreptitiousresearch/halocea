/* hcex_load_all_tex @0x83684260 — force every bitmap tag in the loaded cache to be uploaded to hardware.
 * Iterates the tag database over the 'bitm' group and hands each tag index to hcex_load_bitmap_group.
 * tag_iterator_new / tag_iterator_next are Blam boundary functions. */

#include <stdint.h>
#include "../headers/tag_iterator.h"

extern void tag_iterator_new(tag_iterator *iterator, uint32_t key_group_tag);
extern int tag_iterator_next(tag_iterator *iterator);
extern void hcex_load_bitmap_group(int bitmap_group_index);

extern "C" int hcex_load_all_tex(void)
{
    tag_iterator iterator;
    int bitmap_tag_index;

    tag_iterator_new(&iterator, 0x6269746D /* 'bitm' */);
    for (bitmap_tag_index = tag_iterator_next(&iterator); bitmap_tag_index != -1;
         bitmap_tag_index = tag_iterator_next(&iterator))
    {
        hcex_load_bitmap_group(bitmap_tag_index);
    }
    return bitmap_tag_index;
}
