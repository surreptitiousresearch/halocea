/* bitmap_group_get_bitmap_from_sequence @0x83794128 — resolve a (sequence, frame) pair within a bitmap group to
 * the underlying bitmap_data. A sequence is either a run of bitmaps (first_bitmap_index + bitmap_count, frame
 * wrapped modulo the count) or a sprite sheet (frame indexes the sprites block, each naming its bitmap). When the
 * sequence yields nothing the frame index is used directly as the bitmap index. Returns null on any miss. */

#include <stdint.h>
#include "headers/bitmap_group.h"
#include "headers/bitmap_group_sequence.h"
#include "headers/bitmap_group_sprite.h"
#include "headers/bitmap_data.h"
#include "headers/global_tag_instances.h"

bitmap_data *bitmap_group_get_bitmap_from_sequence(int bitmap_group_index, int16_t sequence_index,
                                                   int16_t frame_index)
{
    if (bitmap_group_index == -1)
        return nullptr;

    bitmap_group *group = TAG_GET(bitmap_group, bitmap_group_index);
    if (!group)
        return nullptr;

    int16_t bitmap_index = -1;
    int sequence_count = group->sequences.count;
    if (sequence_count > 0)
    {
        bitmap_group_sequence *sequence =
            (bitmap_group_sequence *)group->sequences.address + (sequence_index % sequence_count);
        if (sequence->bitmap_count <= 0)
        {
            if (sequence->sprites.count)
                bitmap_index = ((bitmap_group_sprite *)sequence->sprites.address)[frame_index].bitmap_index;
        }
        else
        {
            bitmap_index = frame_index % sequence->bitmap_count + sequence->first_bitmap_index;
        }
    }

    if (bitmap_index == -1)
        bitmap_index = frame_index;

    if (bitmap_index >= 0 && bitmap_index < group->bitmaps.count)
        return &((bitmap_data *)group->bitmaps.address)[bitmap_index];
    return nullptr;
}
