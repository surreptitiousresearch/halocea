/* animation_get_keyframe_translation @0x837965B8 — sample a node's translation from a compressed animation's
 * keyframe stream at a (fractional) frame index. Structurally identical to animation_get_keyframe_rotation, but the
 * per-node default and keyframes are raw real_point3d values (12-byte stride), linearly interpolated rather than
 * slerped. The per-node descriptor and the three data regions live at a different set of block-header words.
 *
 * Reconstructed from disassembly — the decompiler's local allocation failed for this routine. */

#include <stdint.h>
#include <math.h>
#include "headers/animation.h"
#include "headers/animation_compressed_block.h"
#include "headers/real_point3d.h"

extern int16_t animation_keyframe_search(const int16_t *keyframe_frame_indices, int16_t keyframe_count, int16_t target_frame_index);
extern void points_interpolate(const real_point3d *a, const real_point3d *b, float t, real_point3d *result);

void animation_get_keyframe_translation(const animation *animation, float real_frame_index,
                                        int16_t adjusted_node_index, int16_t node_index, real_point3d *result)
{
    const animation_compressed_block *compressed_block =
        (const animation_compressed_block *)((char *)animation->data.address + animation->compressed_data_offset);

    unsigned int descriptor =
        ((const unsigned int *)((const char *)compressed_block
                                + compressed_block->translation_descriptor_offset))[adjusted_node_index];
    const char *keyframe_data_base = (const char *)compressed_block + compressed_block->translation_default_offset;
    short keyframe_count = (short)(descriptor & 0xFFF);
    short data_index = (short)(descriptor >> 12);

    if ( keyframe_count == 0 )
    {
        /* No keyframes — constant per-node translation. */
        /* recovered: (real_point3d *)(base + 12*node_index) -> typed [] index */
        *result = ((const real_point3d *)keyframe_data_base)[node_index];
        return;
    }

    const short *keyframe_frame_indices =
        &((const short *)((const char *)compressed_block
                          + compressed_block->translation_frame_index_offset))[data_index];
    /* recovered: 12*data_index byte offset -> typed real_point3d index off the region base */
    const real_point3d *keyframe_points =
        (const real_point3d *)((const char *)compressed_block
                               + compressed_block->translation_keyframe_offset) + data_index;

    short target_frame_index = (short)(int)floorf(real_frame_index);

    const real_point3d *current_point;
    const real_point3d *next_point = keyframe_points;
    short current_frame_index;
    short next_frame_index;

    if ( target_frame_index < keyframe_frame_indices[0] )
    {
        /* recovered: (real_point3d *)(base + 12*node_index) -> typed [] index */
        current_point = &((const real_point3d *)keyframe_data_base)[node_index];
        current_frame_index = 0;
        next_frame_index = keyframe_frame_indices[0];
    }
    else if ( target_frame_index == keyframe_frame_indices[keyframe_count - 1] )
    {
        current_point = &keyframe_points[keyframe_count - 1];
        current_frame_index = keyframe_frame_indices[keyframe_count - 1];
        /* recovered: (real_point3d *)(base + 12*node_index) -> typed [] index */
        next_point = &((const real_point3d *)keyframe_data_base)[node_index];
        next_frame_index = (short)(keyframe_frame_indices[keyframe_count - 1] + 1);
    }
    else
    {
        short index = animation_keyframe_search(keyframe_frame_indices, keyframe_count, target_frame_index);
        current_point = &keyframe_points[index];
        current_frame_index = keyframe_frame_indices[index];
        next_point = &keyframe_points[index + 1];
        next_frame_index = keyframe_frame_indices[index + 1];
    }

    if ( real_frame_index == (float)current_frame_index )
    {
        *result = *current_point;
    }
    else
    {
        float t = (real_frame_index - (float)current_frame_index)
                / (float)(next_frame_index - current_frame_index);
        points_interpolate(current_point, next_point, t, result);
    }
}
