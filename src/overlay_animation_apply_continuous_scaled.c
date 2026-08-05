/* overlay_animation_apply_continuous_scaled @0x83797B88 — blend an overlay animation into node orientations
 * at a continuous (fractional) frame position, additionally weighted by animation_scale. Combines the two
 * already-reconstructed siblings: the continuous frame-interpolation of overlay_animation_apply_continuous
 * (fractional frame -> two bracketing frames blended by the fractional part) with the scale-weighting of
 * overlay_animation_apply_scaled (interpolate the blended rotation toward identity by animation_scale, scale
 * the translation delta by animation_scale, and blend the scale delta toward 1 by animation_scale).
 *
 * The decompile for this function is corrupted well past what's practical to hand-transcribe (double/QWORD
 * scratch-slot aliasing throughout, plus the same SHIWORD/SWORD1 compressed-quaternion-unpack obfuscation
 * already solved in the two sibling files) but its DB-inferred 4-arg prototype and overall shape (continuous
 * blend + scale weighting, same 3 animated channels) match the siblings exactly, so this reconstruction
 * composes their already-disasm-verified logic rather than re-deriving it from this function's own scramble. */

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
extern void animation_get_keyframe_scale(const animation *animation, float real_frame_index, int16_t adjusted_node_index, float *result);
extern void quaternion_decompress_8byte(const compressed_quaternion_8byte *compressed, real_quaternion *decompressed);
extern void quaternions_interpolate_and_normalize(const real_quaternion *q0, const real_quaternion *q1, float t, real_quaternion *result);
extern void quaternions_interpolate(const real_quaternion *q0, const real_quaternion *q1, float t, real_quaternion *result);
extern void quaternions_multiply(const real_quaternion *q0, const real_quaternion *q1, real_quaternion *result);
extern void points_interpolate(const real_point3d *a, const real_point3d *b, float t, real_point3d *result);
extern void scalars_interpolate(float a, float b, float t, float *result);

extern int animation_is_compressed(const animation *animation);
void overlay_animation_apply_continuous_scaled(const animation *animation, float real_frame_index,
                                               float animation_scale, real_orientation *node_orientations)
{
    float inverse_scale = 1.0f - animation_scale;
    float fraction = (float)fmod((double)real_frame_index, 1.0);
    float real_frame = real_frame_index;
    int frame_count = animation->frame_count;
    int16_t frame_index = (int16_t)(int)(float)floor((double)__fabs(real_frame_index));

    if ( frame_index >= frame_count )
    {
        frame_index = (int16_t)(frame_count - 1);
        fraction = 1.0f;
        real_frame = (float)(frame_count - 1);
    }

    if ( animation->type != animation_overlay )
        return;

    /* DEVIATION: collapsed inlined copy of animation_is_compressed@0x837956E0 (zero-xref donor; sole arg is the local `animation` pointer, no folding needed). */
    uint8_t use_keyframe_accessors = animation_is_compressed(animation);

    int16_t next_index = 0;
    if ( frame_index != frame_count - 1 )
        next_index = (int16_t)(frame_index + 1);

    const char *frame_cursor = animation_get_frame_data(animation, frame_index);
    const char *next_cursor = animation_get_frame_data(animation, next_index);

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
            real_quaternion interpolated_rotation;
            if ( use_keyframe_accessors )
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
            quaternions_interpolate(global_identity_quaternion, &interpolated_rotation, animation_scale,
                                    &interpolated_rotation);
            quaternions_multiply(&interpolated_rotation, &node_orientation->rotation, &node_orientation->rotation);
        }
        rotation_flags >>= 1;

        if ( translation_flags & 1 )
        {
            real_point3d frame_translation;
            if ( use_keyframe_accessors )
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
            node_orientation->translation.x += frame_translation.x * animation_scale;
            node_orientation->translation.y += frame_translation.y * animation_scale;
            node_orientation->translation.z += frame_translation.z * animation_scale;
        }
        translation_flags >>= 1;

        if ( scale_flags & 1 )
        {
            float frame_scale;
            if ( use_keyframe_accessors )
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
            node_orientation->scale = (frame_scale * animation_scale + inverse_scale) * node_orientation->scale;
        }
        scale_flags >>= 1;
    }
}
