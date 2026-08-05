/* overlay_animation_apply_continuous @0x83797760 — blend an overlay animation into an array of node
 * orientations at a continuous (fractional) frame position. The integer floor of real_frame_index selects
 * the base frame, the fractional part is the interpolation factor toward the next frame (wrapping to frame 0
 * at the end). For each animated node, the rotation is interpolated between the two frames and right-
 * multiplied onto the node's current rotation; the translation is interpolated and added; the scale is
 * interpolated and multiplied. Which channels a node carries comes from the per-node rotation/translation/
 * scale bitflag arrays (one bit per node, 32 nodes per dword). Frame data is read either through the
 * keyframe-accessor functions (uncompressed keyframes, when compression is globally enabled or there is no
 * compressed block) or straight from the packed frame stream. Sibling of overlay_animation_apply_scaled.
 *
 * DEVIATIONS (disasm-grounded; the decompiler flagged "local variable allocation failed"):
 *   1. FPR-shadow: real_frame_index is the float 2nd arg (f1); the decompiler invented phantom a4/a5/a6
 *      params and aliased node_orientations as 'a4'. Collapsed back to the 3-arg DB prototype.
 *   2. The packed rotation reads two 8-byte compressed quaternions (one per frame) and feeds them to
 *      quaternions_interpolate_and_normalize; the decompiler rendered this as inline SHIWORD/SWORD1 byte
 *      shuffles with a garbage (v36) result pointer. The real result target is the local interpolated
 *      quaternion (disasm: r7/r6 = stack temp), then quaternions_multiply(interp, node->rotation,
 *      node->rotation).
 *   3. The keyframe-accessor calls' arguments were scrambled (phantom v29/v23). Disasm register trace:
 *      rotation = (animation, (float)frame_index, rotation_adjusted++, node_index, &result);
 *      translation/scale = (animation, real_frame, *_adjusted++, [node_index,] &result).
 *      NOTE the asymmetry, faithful to the binary: the rotation keyframe accessor is given the INTEGER frame
 *      while translation/scale are given the fractional real_frame. */

#include <stdint.h>
#include "headers/animation.h"
#include "headers/animation_flags.h"
#include "headers/real_orientation.h"
#include "headers/real_quaternion.h"
#include "headers/real_point3d.h"
#include "headers/compressed_quaternion_8byte.h"
#include "headers/animation_type.h"
#include "headers/blam_data_globals.h"


extern double fmod(double x, double y);
extern double floor(double x);
extern float __fabs(float x);
extern void *animation_get_frame_data(const animation *animation, int16_t frame_index);
extern void animation_get_keyframe_rotation(const animation *animation, float real_frame_index, int16_t adjusted_node_index, int16_t node_index, real_quaternion *result);
extern void animation_get_keyframe_translation(const animation *animation, float real_frame_index, int16_t adjusted_node_index, int16_t node_index, real_point3d *result);
/* 4 effective args, result in r6; no node_index passed (matches the call site, see scaled sibling) */
extern void animation_get_keyframe_scale(const animation *animation, float real_frame_index, int16_t adjusted_node_index, float *result);
extern void quaternion_decompress_8byte(const compressed_quaternion_8byte *compressed, real_quaternion *decompressed);
extern void quaternions_interpolate_and_normalize(const real_quaternion *q0, const real_quaternion *q1, float t, real_quaternion *result);
extern void quaternions_multiply(const real_quaternion *q0, const real_quaternion *q1, real_quaternion *result);
extern void points_interpolate(const real_point3d *a, const real_point3d *b, float t, real_point3d *result);
extern void scalars_interpolate(float a, float b, float t, float *result);

extern int animation_is_compressed(const animation *animation);
void overlay_animation_apply_continuous(const animation *animation, float real_frame_index,
                                        real_orientation *node_orientations)
{
    float fraction = (float)fmod((double)real_frame_index, 1.0);
    float real_frame = real_frame_index;
    int frame_count = animation->frame_count;
    int16_t frame_index = (int16_t)(int)(float)floor((double)__fabs(real_frame_index));

    if (frame_index >= frame_count)
    {
        frame_index = (int16_t)(frame_count - 1);
        fraction = 1.0f;
        real_frame = (float)(frame_count - 1);
    }

    if (animation->type != animation_overlay)
        return;

    /* DEVIATION: collapsed inlined copy of animation_is_compressed@0x837956E0 (zero-xref donor; sole arg is the local `animation` pointer, no folding needed). */
    uint8_t use_keyframe_accessors = animation_is_compressed(animation);

    int16_t next_index = 0;
    if (frame_index != frame_count - 1)
        next_index = (int16_t)(frame_index + 1);

    const char *frame_cursor = animation_get_frame_data(animation, frame_index);
    const char *next_cursor = animation_get_frame_data(animation, next_index);

    int16_t rotation_adjusted_index = 0;
    int16_t translation_adjusted_index = 0;
    int16_t scale_adjusted_index = 0;

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
            real_quaternion interpolated_rotation;
            if (use_keyframe_accessors)
            {
                animation_get_keyframe_rotation(animation, (float)frame_index, rotation_adjusted_index++,
                                                node_index, &interpolated_rotation);
            }
            else
            {
                real_quaternion frame_rotation, next_rotation;
                quaternion_decompress_8byte((const compressed_quaternion_8byte *)frame_cursor, &frame_rotation);
                frame_cursor += 8;
                quaternion_decompress_8byte((const compressed_quaternion_8byte *)next_cursor, &next_rotation);
                next_cursor += 8;
                quaternions_interpolate_and_normalize(&frame_rotation, &next_rotation, fraction,
                                                      &interpolated_rotation);
            }
            quaternions_multiply(&interpolated_rotation, &node_orientation->rotation,
                                 &node_orientation->rotation);
        }
        rotation_flags >>= 1;

        if (translation_flags & 1)
        {
            real_point3d frame_translation;
            if (use_keyframe_accessors)
            {
                animation_get_keyframe_translation(animation, real_frame, translation_adjusted_index++,
                                                   node_index, &frame_translation);
            }
            else
            {
                points_interpolate((const real_point3d *)frame_cursor, (const real_point3d *)next_cursor,
                                   fraction, &frame_translation);
                frame_cursor += 12;
                next_cursor += 12;
            }
            node_orientation->translation.x += frame_translation.x;
            node_orientation->translation.y += frame_translation.y;
            node_orientation->translation.z += frame_translation.z;
        }
        translation_flags >>= 1;

        if (scale_flags & 1)
        {
            float frame_scale;
            if (use_keyframe_accessors)
            {
                animation_get_keyframe_scale(animation, real_frame, scale_adjusted_index++, &frame_scale);
            }
            else
            {
                scalars_interpolate(*(const float *)frame_cursor, *(const float *)next_cursor, fraction,
                                    &frame_scale);
                frame_cursor += 4;
                next_cursor += 4;
            }
            node_orientation->scale = node_orientation->scale * frame_scale;
        }
        scale_flags >>= 1;
    }
}
