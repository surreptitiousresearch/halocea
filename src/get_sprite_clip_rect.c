/* get_sprite_clip_rect @0x8379E080 — the normalized clip rect for one animation frame of a sprite-sheet
 * sequence, or nullptr when the bitmap group / sequence / frame is invalid or the sequence isn't a sprite
 * sheet (no sprites block). Frame indices wrap modulo the sprite count. */

#include "headers/bitmap_group.h"
#include "headers/bitmap_group_sequence.h"
#include "headers/bitmap_group_sprite.h"
#include "headers/global_tag_instances.h"
#include <stdint.h>

const real_rectangle2d *get_sprite_clip_rect(int bitmap_group_index, int16_t sequence_index, int16_t frame)
{
    if (bitmap_group_index == -1 || sequence_index == -1 || frame == -1)
        return nullptr;

    bitmap_group *group = TAG_GET(bitmap_group, bitmap_group_index);
    if (sequence_index >= group->sequences.count)
        return nullptr;

    bitmap_group_sequence *sequence = (bitmap_group_sequence *)group->sequences.address + sequence_index;
    int sprite_count = sequence->sprites.count;
    if (!sprite_count)
        return nullptr;

    bitmap_group_sprite *sprite = (bitmap_group_sprite *)sequence->sprites.address + (frame % sprite_count);
    return &sprite->bounds;
}
