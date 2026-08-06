/* animation_get_keyframe_rotation @0x83796390 — sample a node's rotation from a compressed animation's keyframe
 * stream at a (fractional) frame index. The compressed block holds, per node, a descriptor word packing a keyframe
 * count and a data index; the keyframes are stored as parallel arrays of frame indices (int16) and 6-byte
 * compressed quaternions. The result is the keyframe at the frame, or a slerp between the bracketing keyframes
 * (with the per-node default pose acting as the keyframe before frame 0 and after the last keyframe).
 *
 * Reconstructed from disassembly — the decompiler's local allocation failed (overlapped scratch + phantom args). */

#include <stdint.h>
#include <math.h>
#include "headers/animation.h"
#include "headers/animation_compressed_block.h"
#include "headers/real_quaternion.h"
#include "headers/compressed_quaternion_6byte.h"

extern void quaternion_decompress_6byte(const compressed_quaternion_6byte *compressed, real_quaternion *decompressed);
extern void quaternion_normalize(real_quaternion *q);
extern int16_t animation_keyframe_search(const int16_t *keyframe_frame_indices, int16_t keyframe_count, int16_t target_frame_index);
extern void quaternions_interpolate_and_normalize(const real_quaternion *q0, const real_quaternion *q1, float t, real_quaternion *result);

void animation_get_keyframe_rotation(const animation *animation, float real_frame_index,
                                     int16_t adjusted_node_index, int16_t node_index, real_quaternion *result)
{
    const animation_compressed_block *compressed_block =
        (const animation_compressed_block *)((char *)animation->data.address + animation->compressed_data_offset);

    unsigned int descriptor = compressed_block->rotation_descriptors[adjusted_node_index];
    const char *keyframe_data_base = (const char *)compressed_block + compressed_block->rotation_default_offset;
    short keyframe_count = (short)(descriptor & 0xFFF);
    short data_index = (short)(descriptor >> 12);

    if ( keyframe_count == 0 )
    {
        /* No keyframes — constant per-node pose. */
        /* recovered: (compressed_quaternion_6byte *)(base + 6*node_index) -> typed [] index */
        quaternion_decompress_6byte(
            &((const compressed_quaternion_6byte *)keyframe_data_base)[node_index], result);
        quaternion_normalize(result);
        return;
    }

    const short *keyframe_frame_indices =
        &((const short *)((const char *)compressed_block
                          + compressed_block->rotation_frame_index_offset))[data_index];
    /* recovered: 6*data_index byte offset -> typed compressed_quaternion_6byte index off the region base */
    const compressed_quaternion_6byte *keyframe_quaternions =
        &((const compressed_quaternion_6byte *)((const char *)compressed_block
                                                + compressed_block->rotation_keyframe_offset))[data_index];

    short target_frame_index = (short)(int)floorf(real_frame_index);

    const compressed_quaternion_6byte *current_keyframe;
    const compressed_quaternion_6byte *next_keyframe = keyframe_quaternions;
    short current_frame_index;
    short next_frame_index;

    if ( target_frame_index < keyframe_frame_indices[0] )
    {
        /* Before the first keyframe: ramp from the per-node default pose toward keyframe 0. */
        current_keyframe = &((const compressed_quaternion_6byte *)keyframe_data_base)[node_index];
        current_frame_index = 0;
        next_frame_index = keyframe_frame_indices[0];
    }
    else if ( target_frame_index == keyframe_frame_indices[keyframe_count - 1] )
    {
        /* At the last keyframe: ramp from it toward the per-node default pose. */
        current_keyframe = &keyframe_quaternions[keyframe_count - 1];
        current_frame_index = keyframe_frame_indices[keyframe_count - 1];
        next_keyframe = &((const compressed_quaternion_6byte *)keyframe_data_base)[node_index];
        next_frame_index = (short)(keyframe_frame_indices[keyframe_count - 1] + 1);
    }
    else
    {
        short index = animation_keyframe_search(keyframe_frame_indices, keyframe_count, target_frame_index);
        current_keyframe = &keyframe_quaternions[index];
        current_frame_index = keyframe_frame_indices[index];
        next_keyframe = &keyframe_quaternions[index + 1];
        next_frame_index = keyframe_frame_indices[index + 1];
    }

    if ( real_frame_index == (float)current_frame_index )
    {
        quaternion_decompress_6byte(current_keyframe, result);
        quaternion_normalize(result);
    }
    else
    {
        real_quaternion current_orientation;
        real_quaternion next_orientation;
        quaternion_decompress_6byte(current_keyframe, &current_orientation);
        quaternion_decompress_6byte(next_keyframe, &next_orientation);
        float t = (real_frame_index - (float)current_frame_index)
                / (float)(next_frame_index - current_frame_index);
        quaternions_interpolate_and_normalize(&current_orientation, &next_orientation, t, result);
    }
}
