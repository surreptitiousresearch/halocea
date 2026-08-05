/* postprocess_bitmap_group @0x83793EC8 — finalize a freshly loaded bitmap tag group. Every bitmap in the group
 * is (for interface-usage groups) flagged for the UI, verified, and registered with the texture cache; a bitmap
 * that fails verification makes the whole group fail. For sprite-usage groups any stale registration point on a
 * sequence is cleared, and a trailing all-empty sequence is trimmed off the sequence block. Returns 1 on
 * success, 0 if any bitmap failed verification or the trailing-sequence trim failed. `editing` is accepted but
 * not used by this pass.
 *
 * The bitmap group definition is kept as raw tag offsets: usage word @+0, sequence block @+84 (count/+88
 * address, 64-byte stride), bitmap block @+96 (count/+100 address, 48-byte bitmap_data stride). Under the
 * find_all_fucked_up_shit debug switch the shipped release build runs empty validation loops (the asserts were
 * compiled out); they are reproduced for fidelity. */

#include <stdint.h>
#include "headers/global_tag_instances.h"
#include "headers/bitmap_data.h"
#include "headers/bitmap_flags.h"
#include "headers/bitmap_group.h"
#include "headers/bitmap_group_type.h"
#include "headers/bitmap_group_sequence.h"
#include "headers/tag_block.h"
#include "headers/blam_data_globals.h"


extern uint8_t bitmap_verify(const bitmap_data *bitmap, uint8_t import);
extern void texture_cache_bitmap_new(int tag_index, bitmap_data *bitmap);
extern uint8_t tag_block_resize(tag_block *block, int element_count);

int postprocess_bitmap_group(int bitmap_group_index, uint8_t editing)
{
    int result = 1;
    bitmap_group *bitmap_group_def = TAG_GET(bitmap_group, bitmap_group_index);

    if ( bitmap_group_def->bitmaps.count > 0 )
    {
        int bitmap_index = 0;
        do
        {
            bitmap_data *bitmap = &((bitmap_data *)bitmap_group_def->bitmaps.address)[bitmap_index];
            if ( bitmap_group_def->type == _bitmap_group_type_interface_bitmaps )
                bitmap->flags |= (1u << _bitmap_linear_bit);
            if ( bitmap_verify(bitmap, 0) )
                texture_cache_bitmap_new(bitmap_group_index, bitmap);
            else
                result = 0;
            bitmap_index = (int16_t)(bitmap_index + 1);
        }
        while ( bitmap_index < bitmap_group_def->bitmaps.count );
    }

    if ( bitmap_group_def->sequences.count > 0 )
    {
        bitmap_group_sequence *sequences = (bitmap_group_sequence *)bitmap_group_def->sequences.address;
        int sequence_count;
        int sequence_index = 0;
        do
        {
            bitmap_group_sequence *sequence = &sequences[sequence_index];
            if ( bitmap_group_def->type == _bitmap_group_type_sprites
              && (sequence->first_bitmap_index || sequence->bitmap_count) )
            {
                sequence->first_bitmap_index = 0;
                sequence->bitmap_count = 0;
            }
            sequence_count = bitmap_group_def->sequences.count;
            sequence_index = (int16_t)(sequence_index + 1);
        }
        while ( sequence_index < sequence_count );

        if ( sequence_count > 0 )
        {
            bitmap_group_sequence *last_sequence = &sequences[sequence_count - 1];
            if ( !last_sequence->bitmap_count && !last_sequence->sprites.count )
            {
                if ( tag_block_resize(&bitmap_group_def->sequences, sequence_count - 1) == 0 )
                    result = 0;
            }
        }
    }

    if ( find_all_fucked_up_shit )
    {
        int bitmap_count = bitmap_group_def->bitmaps.count;
        for ( int i = 0; i < bitmap_count; i++ )
        {
            /* compiled-out per-bitmap validation */
        }
        if ( bitmap_group_def->sequences.count > 0 )
        {
            bitmap_group_sequence *sequences = (bitmap_group_sequence *)bitmap_group_def->sequences.address;
            for ( int sequence_index = 0; sequence_index < bitmap_group_def->sequences.count; sequence_index++ )
            {
                if ( bitmap_group_def->type == _bitmap_group_type_sprites )
                {
                    int sprite_count = sequences[sequence_index].sprites.count;
                    for ( int sprite_index = 0; sprite_index < sprite_count; sprite_index++ )
                    {
                        /* compiled-out per-sprite validation */
                    }
                }
            }
        }
    }

    return result;
}
