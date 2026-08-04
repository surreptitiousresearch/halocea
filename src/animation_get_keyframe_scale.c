/* animation_get_keyframe_scale @0x83796800 — sample a node's uniform scale from a compressed animation's keyframe
 * stream at a (fractional) frame index. Structurally identical to the rotation/translation samplers, but values are
 * scalar floats (4-byte stride) interpolated with scalars_interpolate. This variant has no separate node_index
 * parameter: the per-node default scale is keyed by adjusted_node_index, and the result pointer arrives in r6.
 *
 * Reconstructed from disassembly — the decompiler's local allocation failed for this routine. */

#include <stdint.h>
#include <math.h>
#include "headers/animation.h"

extern int16_t animation_keyframe_search(const int16_t *keyframe_frame_indices, int16_t keyframe_count, int16_t target_frame_index);
extern void scalars_interpolate(float a, float b, float t, float *result);

void animation_get_keyframe_scale(const animation *animation, float real_frame_index,
                                  int16_t adjusted_node_index, float *result)
{
    unsigned int *compressed_block =
        (unsigned int *)((char *)animation->data.address + animation->compressed_data_offset);

    /* RAW: packed codec stream — compressed_block[7] is a byte offset word in the block header; the
     * per-node descriptor table it points to has no DB struct. Byte-exact index kept. */
    unsigned int descriptor = *(unsigned int *)((char *)compressed_block + 4 * adjusted_node_index + compressed_block[7]);
    char *keyframe_data_base = (char *)compressed_block + compressed_block[9];
    short keyframe_count = (short)(descriptor & 0xFFF);
    short data_index = (short)(descriptor >> 12);

    if ( keyframe_count == 0 )
    {
        /* No keyframes — constant per-node scale. */
        *result = ((const float *)keyframe_data_base)[adjusted_node_index];
        return;
    }

    /* RAW: same packed codec stream — words [8]/[10] are header byte-offset words to the frame-index
     * and value tables; no DB struct exists for the block layout, byte-exact arithmetic kept. */
    const short *keyframe_frame_indices =
        (const short *)((char *)compressed_block + 2 * data_index + compressed_block[8]);
    const float *keyframe_values =
        (const float *)((char *)compressed_block + 4 * data_index + compressed_block[10]);

    short target_frame_index = (short)(int)floorf(real_frame_index);

    float current_value;
    float next_value;
    short current_frame_index;
    short next_frame_index;

    if ( target_frame_index < keyframe_frame_indices[0] )
    {
        current_value = ((const float *)keyframe_data_base)[adjusted_node_index];
        current_frame_index = 0;
        next_value = keyframe_values[0];
        next_frame_index = keyframe_frame_indices[0];
    }
    else if ( target_frame_index == keyframe_frame_indices[keyframe_count - 1] )
    {
        current_value = keyframe_values[keyframe_count - 1];
        current_frame_index = keyframe_frame_indices[keyframe_count - 1];
        next_value = ((const float *)keyframe_data_base)[adjusted_node_index];
        next_frame_index = (short)(keyframe_frame_indices[keyframe_count - 1] + 1);
    }
    else
    {
        short index = animation_keyframe_search(keyframe_frame_indices, keyframe_count, target_frame_index);
        current_value = keyframe_values[index];
        current_frame_index = keyframe_frame_indices[index];
        next_value = keyframe_values[index + 1];
        next_frame_index = keyframe_frame_indices[index + 1];
    }

    if ( real_frame_index == (float)current_frame_index )
    {
        *result = current_value;
    }
    else
    {
        float t = (real_frame_index - (float)current_frame_index)
                / (float)(next_frame_index - current_frame_index);
        scalars_interpolate(current_value, next_value, t, result);
    }
}
