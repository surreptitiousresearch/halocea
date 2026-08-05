/* overlay_animation_apply @0x837971E8 — apply an overlay animation's exact (non-interpolated) frame onto an
 * array of node orientations. Sibling of overlay_animation_apply_continuous, which blends between two
 * frames; this one reads a single frame's rotation/translation/scale straight from the packed stream (or via
 * the keyframe accessors when compression allows it) with no interpolation factor.
 *
 * DEVIATION: the decompiler produced heavily scrambled variable names for this function (unlike the
 * continuous sibling, which at least kept clear names despite its own "local variable allocation failed"
 * flag). Rebuilt from disasm using the same register mapping already established for the sibling:
 * animation_get_keyframe_rotation/translation take (animation, real_frame_index, adjusted_node_index,
 * node_index, result) — 5 args, node_index in r6 and result in r7; animation_get_keyframe_scale takes only
 * (animation, real_frame_index, adjusted_node_index, result) — 4 args, result in r6. */

#include <stdint.h>
#include "headers/animation.h"
#include "headers/animation_flags.h"
#include "headers/real_orientation.h"
#include "headers/real_quaternion.h"
#include "headers/real_point3d.h"
#include "headers/compressed_quaternion_8byte.h"
#include "headers/animation_type.h"
#include "headers/blam_data_globals.h"


extern void *animation_get_frame_data(const animation *animation, int16_t frame_index);
extern void animation_get_keyframe_rotation(const animation *animation, float real_frame_index, int16_t adjusted_node_index, int16_t node_index, real_quaternion *result);
extern void animation_get_keyframe_translation(const animation *animation, float real_frame_index, int16_t adjusted_node_index, int16_t node_index, real_point3d *result);
extern void animation_get_keyframe_scale(const animation *animation, float real_frame_index, int16_t adjusted_node_index, float *result);
extern void quaternion_decompress_8byte(const compressed_quaternion_8byte *compressed, real_quaternion *decompressed);
extern void quaternions_multiply(const real_quaternion *q0, const real_quaternion *q1, real_quaternion *result);

extern int animation_is_compressed(const animation *animation);
void overlay_animation_apply(const animation *animation, int16_t frame_index, real_orientation *node_orientations)
{
    if ( animation->type != animation_overlay )
        return;
    if ( frame_index < 0 || frame_index >= animation->frame_count )
        return;

    /* DEVIATION: collapsed inlined copy of animation_is_compressed@0x837956E0 (zero-xref donor; sole arg is the local `animation` pointer, no folding needed). */
    uint8_t use_keyframe_accessors = animation_is_compressed(animation);

    const char *frame_cursor = animation_get_frame_data(animation, frame_index);

    int16_t rotation_adjusted_index = 0;
    int16_t translation_adjusted_index = 0;
    int16_t scale_adjusted_index = 0;

    unsigned int rotation_flags = 0;
    unsigned int translation_flags = 0;
    unsigned int scale_flags = 0;

    for ( int16_t node_index = 0; node_index < animation->node_count; node_index = (int16_t)(node_index + 1) )
    {
        real_orientation *node_orientation = &node_orientations[node_index];

        if ( (node_index & 0x1F) == 0 )
        {
            int word_index = node_index >> 5;
            translation_flags = animation->nodes_with_translation_flags[word_index];
            rotation_flags = animation->nodes_with_rotation_flags[word_index];
            scale_flags = animation->nodes_with_scale_flags[word_index];
        }

        if ( rotation_flags & 1 )
        {
            real_quaternion frame_rotation;
            if ( use_keyframe_accessors )
            {
                animation_get_keyframe_rotation(animation, (float)frame_index, rotation_adjusted_index++,
                                                node_index, &frame_rotation);
            }
            else
            {
                quaternion_decompress_8byte((const compressed_quaternion_8byte *)frame_cursor, &frame_rotation);
                frame_cursor += 8;
            }
            quaternions_multiply(&frame_rotation, &node_orientation->rotation, &node_orientation->rotation);
        }
        rotation_flags >>= 1;

        if ( translation_flags & 1 )
        {
            real_point3d frame_translation;
            if ( use_keyframe_accessors )
            {
                animation_get_keyframe_translation(animation, (float)frame_index, translation_adjusted_index++,
                                                   node_index, &frame_translation);
            }
            else
            {
                frame_translation = *(const real_point3d *)frame_cursor;
                frame_cursor += 12;
            }
            node_orientation->translation.x += frame_translation.x;
            node_orientation->translation.y += frame_translation.y;
            node_orientation->translation.z += frame_translation.z;
        }
        translation_flags >>= 1;

        if ( scale_flags & 1 )
        {
            float frame_scale;
            if ( use_keyframe_accessors )
            {
                animation_get_keyframe_scale(animation, (float)frame_index, scale_adjusted_index++, &frame_scale);
            }
            else
            {
                frame_scale = *(const float *)frame_cursor;
                frame_cursor += 4;
            }
            node_orientation->scale = node_orientation->scale * frame_scale;
        }
        scale_flags >>= 1;
    }
}
