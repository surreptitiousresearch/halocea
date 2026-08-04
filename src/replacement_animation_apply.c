/* replacement_animation_apply @0x83796F98 — overlay a "replacement" (type 2) animation's frame onto a node
 * pose array. For each animated node it applies the components (rotation, translation, uniform scale) that the
 * animation drives, as indicated by the per-component node-flag bitfields (one 32-bit word per 32 nodes). When
 * the animation carries compressed keyframe data (and compression is enabled, or it has no separate compressed
 * block) the keyframe accessor API is used, walking a running per-component index; otherwise the raw frame data
 * is consumed inline — an 8-byte compressed quaternion for rotation, three floats for translation, one float
 * for scale — advancing the frame cursor by the size read. No-op for non-type-2 animations or out-of-range
 * frames.
 *
 * DEVIATION: the decompiler modeled the node loop counter and the (float)frame_index conversion as an __int64
 * register pun and lost the call arguments. Disasm (0x83796FB4-0x837971D8) settles them: real_frame_index =
 * (float)frame_index; the keyframe rotation/translation calls take (adjusted index = running component counter,
 * node_index, result); keyframe_scale takes only (adjusted index, result) (its DB prototype's extra node_index
 * is spurious — matches the 4-arg corpus declaration). The flag words live at animation dword offsets 23/27/31
 * (nodes_with_translation/rotation/scale_flags), indexed by node_index/32. The initial flag values are a dead
 * uninitialized-stack read (node 0 always reloads); reproduced here as 0. */

#include <stdint.h>
#include "headers/animation.h"
#include "headers/animation_flags.h"
#include "headers/real_orientation.h"
#include "headers/compressed_quaternion_8byte.h"
#include "headers/animation_type.h"
#include "headers/blam_data_globals.h"


#include "headers/real_point3d.h"
#include "headers/real_quaternion.h"
extern void *animation_get_frame_data(const animation *animation, int16_t frame_index);
extern void animation_get_keyframe_rotation(const animation *animation, float real_frame_index, int16_t adjusted_node_index, int16_t node_index, real_quaternion *result);
extern void animation_get_keyframe_translation(const animation *animation, float real_frame_index, int16_t adjusted_node_index, int16_t node_index, real_point3d *result);
extern void animation_get_keyframe_scale(const animation *animation, float real_frame_index, int16_t adjusted_node_index, float *result);
extern void quaternion_decompress_8byte(const compressed_quaternion_8byte *compressed, real_quaternion *decompressed);

extern int animation_is_compressed(const animation *animation);
void replacement_animation_apply(const animation *animation, int16_t frame_index, real_orientation *node_orientations)
{
    if ( animation->type != animation_replacement )
        return;
    if ( frame_index < 0 || frame_index >= animation->frame_count )
        return;

    /* DEVIATION: collapsed inlined copy of animation_is_compressed@0x837956E0 (zero-xref donor; sole arg is the local `animation` pointer, no folding needed). */
    bool use_keyframe_api = animation_is_compressed(animation);

    char *frame_cursor = animation_get_frame_data(animation, frame_index);

    int rotation_index = 0;
    int translation_index = 0;
    int scale_index = 0;

    unsigned int rotation_flags = 0;    /* reloaded when (node_index & 0x1F) == 0; initial value is a dead read */
    unsigned int translation_flags = 0;
    unsigned int scale_flags = 0;

    for ( int node_index = 0; node_index < animation->node_count; node_index = (__int16)(node_index + 1) )
    {
        real_orientation *orientation = &node_orientations[node_index];

        if ( (node_index & 0x1F) == 0 )
        {
            int flag_dword = (__int16)node_index >> 5;
            translation_flags = animation->nodes_with_translation_flags[flag_dword];
            rotation_flags = animation->nodes_with_rotation_flags[flag_dword];
            scale_flags = animation->nodes_with_scale_flags[flag_dword];
        }

        if ( rotation_flags & 1 )
        {
            if ( use_keyframe_api )
            {
                animation_get_keyframe_rotation(animation, (float)frame_index, rotation_index++, node_index,
                        &orientation->rotation);
            }
            else
            {
                quaternion_decompress_8byte((const compressed_quaternion_8byte *)frame_cursor,
                        &orientation->rotation);
                frame_cursor += 8;
            }
        }
        rotation_flags >>= 1;

        if ( translation_flags & 1 )
        {
            if ( use_keyframe_api )
            {
                animation_get_keyframe_translation(animation, (float)frame_index, translation_index++, node_index,
                        &orientation->translation);
            }
            else
            {
                /* packed inline frame data (no struct): three consecutive floats */
                orientation->translation.n[0] = ((float *)frame_cursor)[0];
                orientation->translation.n[1] = ((float *)frame_cursor)[1];
                orientation->translation.n[2] = ((float *)frame_cursor)[2];
                frame_cursor += 12;
            }
        }
        translation_flags >>= 1;

        if ( scale_flags & 1 )
        {
            if ( use_keyframe_api )
            {
                animation_get_keyframe_scale(animation, (float)frame_index, scale_index++, &orientation->scale);
            }
            else
            {
                orientation->scale = *(float *)frame_cursor;
                frame_cursor += 4;
            }
        }
        scale_flags >>= 1;
    }
}
