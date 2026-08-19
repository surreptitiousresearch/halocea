/* overlay_animation_apply_scaled @0x83797488 — additively blend one overlay-animation frame into an array of
 * node orientations, weighted by animation_scale. For each animated node: its rotation is slerped from identity
 * toward the frame rotation by animation_scale and right-multiplied onto the node's current rotation; its
 * translation is added scaled by animation_scale; its scale is multiplied by
 * (frame_scale*animation_scale + (1 - animation_scale)). Which of the three channels a node carries is selected
 * by the animation's per-node rotation/translation/scale bitflag arrays (one bit per node, 32 nodes per dword).
 * Frame data is read either through the keyframe-accessor functions (when the animation holds uncompressed
 * keyframes and either compression is globally enabled or there is no compressed block) or straight from the
 * packed frame stream.
 *
 * DEVIATION 1 (FPR-shadow): the float arg animation_scale lives in f1 and shadows GPR slot r5, so the real
 * node_orientations pointer arrives in r6. The decompiler invented a phantom 4th pointer param and exposed the
 * real one as 'a5'; collapsed back to the 4-arg prototype here (node_orientations == r6).
 * DEVIATION 2: animation_get_keyframe_scale is called with only 4 effective args — its result pointer is passed
 * in r6, and the loop node index is NOT supplied. The DB prototype lists an extra node_index that the call site
 * does not pass, so the extern below matches the actual call.
 * DEVIATION 3: the real_frame_index float args are produced by fcfid (int->float); restored as plain
 * (float)frame_index casts rather than the decompiler's __int64/double scratch shuffles. */

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
/* DEVIATION 2: 4 effective args, result in r6; no node_index passed */
extern void animation_get_keyframe_scale(const animation *animation, float real_frame_index, int16_t adjusted_node_index, float *result);
extern void quaternion_decompress_8byte(const compressed_quaternion_8byte *compressed, real_quaternion *decompressed);
extern void quaternions_interpolate(const real_quaternion *q0, const real_quaternion *q1, float t, real_quaternion *result);
extern void quaternions_multiply(const real_quaternion *q0, const real_quaternion *q1, real_quaternion *result);

extern int animation_is_compressed(const animation *animation);
void overlay_animation_apply_scaled(const animation *animation, int16_t frame_index, float animation_scale,
                                    real_orientation *node_orientations)
{
    float inverse_scale = 1.0f - animation_scale;

    if (animation->type != animation_overlay)
        return;
    if (frame_index < 0 || frame_index >= animation->frame_count)
        return;

    /* DEVIATION: collapsed inlined copy of animation_is_compressed@0x837956E0 (zero-xref donor; sole arg is the local `animation` pointer, no folding needed). */
    uint8_t use_keyframe_accessors = animation_is_compressed(animation);

    const char *frame_cursor = animation_get_frame_data(animation, frame_index);

    int16_t rotation_adjusted_index = 0;
    int16_t translation_adjusted_index = 0;
    int16_t scale_adjusted_index = 0;

    if (animation->node_count <= 0)
        return;

    unsigned int rotation_flags = 0;
    unsigned int translation_flags = 0;
    unsigned int scale_flags = 0;

    for (int16_t node_index = 0; node_index < animation->node_count; node_index = (int16_t)(node_index + 1))
    {
        real_orientation *node_orientation = &node_orientations[node_index];

        if ((node_index & 0x1F) == 0)
        {
            int word_index = node_index >> 5;
            translation_flags = animation->nodes_with_translation_flags[word_index];
            rotation_flags = animation->nodes_with_rotation_flags[word_index];
            scale_flags = animation->nodes_with_scale_flags[word_index];
        }

        if (rotation_flags & 1)
        {
            real_quaternion frame_rotation;
            if (use_keyframe_accessors)
            {
                animation_get_keyframe_rotation(animation, (float)frame_index, rotation_adjusted_index++,
                                                node_index, &frame_rotation);
            }
            else
            {
                quaternion_decompress_8byte((const compressed_quaternion_8byte *)frame_cursor, &frame_rotation);
                frame_cursor += 8;
            }
            quaternions_interpolate(global_identity_quaternion, &frame_rotation, animation_scale, &frame_rotation);
            quaternions_multiply(&frame_rotation, &node_orientation->rotation, &node_orientation->rotation);
        }
        rotation_flags >>= 1;

        if (translation_flags & 1)
        {
            real_point3d frame_translation;
            if (use_keyframe_accessors)
            {
                animation_get_keyframe_translation(animation, (float)frame_index, translation_adjusted_index++,
                                                   node_index, &frame_translation);
            }
            else
            {
                frame_translation = *(const real_point3d *)frame_cursor;
                frame_cursor += 12;
            }
            node_orientation->translation.x += frame_translation.x * animation_scale;
            node_orientation->translation.y += frame_translation.y * animation_scale;
            node_orientation->translation.z += frame_translation.z * animation_scale;
        }
        translation_flags >>= 1;

        if (scale_flags & 1)
        {
            float frame_scale;
            if (use_keyframe_accessors)
            {
                animation_get_keyframe_scale(animation, (float)frame_index, scale_adjusted_index++, &frame_scale);
            }
            else
            {
                frame_scale = *(const float *)frame_cursor;
                frame_cursor += 4;
            }
            node_orientation->scale = (frame_scale * animation_scale + inverse_scale) * node_orientation->scale;
        }
        scale_flags >>= 1;
    }
}
