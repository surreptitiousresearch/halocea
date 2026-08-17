/* hud_retrieve_bitmap_and_bounding_rect @0x8379E4E8 */
/* hud_retrieve_bitmap_and_bounding_rect 0x8379E4E8 — resolve a HUD (sequence, frame) within a bitmap group to its
 * bitmap_data and sprite clip rectangle. Sprite-sheet sequences index the sprites block directly (frame wrapped
 * modulo the sprite count) to name the bitmap; other sequences defer to bitmap_group_get_bitmap_from_sequence.
 * The clip rect comes from get_sprite_clip_rect when a bitmap was found, else null. Both outputs are caller-owned
 * pointers; *bitmap must be pre-initialized (it is only written on a successful sprite lookup). */

#include <stdint.h>
#include "headers/bitmap_group.h"
#include "headers/bitmap_group_sequence.h"
#include "headers/bitmap_group_sprite.h"
#include "headers/bitmap_data.h"
#include "headers/real_rectangle2d.h"
#include "headers/global_tag_instances.h"

extern bitmap_data *bitmap_group_get_bitmap_from_sequence(int bitmap_group_index, int16_t sequence_index, int16_t frame_index);
extern const real_rectangle2d *get_sprite_clip_rect(int bitmap_group_index, int16_t sequence_index, int16_t frame);

void hud_retrieve_bitmap_and_bounding_rect(int bitmap_group_index, int16_t sequence_index, int16_t frame_index,
                                           const bitmap_data **bitmap, const real_rectangle2d **clip)
{
    int16_t frame = frame_index;
    if ( bitmap_group_index != -1 )
    {
        bitmap_group *group = TAG_GET(bitmap_group, bitmap_group_index);
        if ( sequence_index < group->sequences.count )
        {
            frame = frame_index & 0x7FFF;
            bitmap_group_sequence *sequence = (bitmap_group_sequence *)group->sequences.address + sequence_index;
            int sprite_count = sequence->sprites.count;
            if ( sprite_count )
                *bitmap = &((bitmap_data *)group->bitmaps.address)
                              [((bitmap_group_sprite *)sequence->sprites.address)[frame % sprite_count].bitmap_index];
            else
                *bitmap = bitmap_group_get_bitmap_from_sequence(bitmap_group_index, sequence_index, frame);
        }
    }

    if ( *bitmap )
        *clip = get_sprite_clip_rect(bitmap_group_index, sequence_index, frame);
    else
        *clip = nullptr;
}
