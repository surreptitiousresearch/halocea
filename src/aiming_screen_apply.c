/* aiming_screen_apply @0x83797FD8 — blend an aiming-screen overlay animation into node orientations by
 * bilinear interpolation over the animation's yaw x pitch frame grid. The aiming-screen bounds give the
 * per-frame angular deltas and frame counts in each of the four directions; direction/elevation are turned
 * into a fractional grid coordinate (cell index + fraction, clamped to the grid), the four surrounding
 * corner frames are fetched, and per node the corner rotations are slerp'd (yaw pair, then pitch) and
 * multiplied into the node rotation while the corner translations are bilinearly blended and added.
 *
 * DEVIATION (reg-alloc-fail rebuild): the decompiler flagged "local variable allocation has failed" and
 * fabricated a6/a7/a8 params from the two float args (direction f1 / elevation f2 burn the r5/r6 shadow
 * slots) — the DB prototype is 5-arg and the real node_orientations is r7 (the decompiler's `a8`). All
 * garbled call ABIs rebuilt from disasm 0x83798350-0x8379855C: animation_get_keyframe_rotation/translation
 * take (animation, real_frame_index, adjusted_node_index r5, node_index r6, result r7), corner results land
 * in the same four stack slots the compressed-stream path fills (corner00/10/01/11), and the fmod register
 * pun (`*((double*)&v23+1) = 1.0`) is simply fmod(t, 1.0). The `(float)corner_index` conversions are the
 * PPC std/lfd+fcfid+frsp int-to-float idiom. */

#include <stdint.h>
#include "headers/animation.h"
#include "headers/animation_flags.h"
#include "headers/animation_aiming_screen_bounds.h"
#include "headers/real_orientation.h"
#include "headers/real_quaternion.h"
#include "headers/real_point3d.h"
#include "headers/compressed_quaternion_8byte.h"
#include "headers/animation_type.h"
#include "headers/blam_data_globals.h"


extern double fmod(double x, double y);
extern void *animation_get_frame_data(const animation *animation, int16_t frame_index);
extern void animation_get_keyframe_rotation(const animation *animation, float real_frame_index, int16_t adjusted_node_index, int16_t node_index, real_quaternion *result);
extern void animation_get_keyframe_translation(const animation *animation, float real_frame_index, int16_t adjusted_node_index, int16_t node_index, real_point3d *result);
extern void quaternion_decompress_8byte(const compressed_quaternion_8byte *compressed, real_quaternion *decompressed);
extern void quaternions_interpolate_and_normalize(const real_quaternion *q0, const real_quaternion *q1, float t, real_quaternion *result);
extern void quaternions_multiply(const real_quaternion *q0, const real_quaternion *q1, real_quaternion *result);

extern int animation_is_compressed(const animation *animation);
void aiming_screen_apply(const animation *animation, const animation_aiming_screen_bounds *aiming_screen,
                         float direction, float elevation, real_orientation *node_orientations)
{
    int16_t grid_width = aiming_screen->positive_yaw_frame_count + aiming_screen->negative_yaw_frame_count + 1;
    int16_t grid_height = aiming_screen->positive_pitch_frame_count + aiming_screen->negative_pitch_frame_count + 1;

    if ( animation->type != animation_overlay )
        return;
    if ( animation->frame_count < grid_width * grid_height )
        return;

    /* DEVIATION: collapsed inlined copy of animation_is_compressed@0x837956E0 (zero-xref donor; sole arg is the local `animation` pointer, no folding needed). */
    uint8_t use_keyframe_accessors = animation_is_compressed(animation);

    /* direction -> yaw cell index + fraction */
    float yaw_delta = direction >= 0.0f ? aiming_screen->positive_yaw_delta
                                        : aiming_screen->negative_yaw_delta;
    float yaw_t = yaw_delta == 0.0f ? 0.0f : direction / yaw_delta;
    int16_t yaw_index = (int16_t)yaw_t;
    float yaw_fraction = (float)fmod(yaw_t, 1.0);
    if ( yaw_fraction < 0.0f )
    {
        yaw_fraction = yaw_fraction + 1.0f;
        yaw_index--;
    }
    if ( yaw_index >= aiming_screen->positive_yaw_frame_count )
    {
        yaw_fraction = 1.0f;
        yaw_index = aiming_screen->positive_yaw_frame_count - 1;
    }
    if ( yaw_index < -aiming_screen->negative_yaw_frame_count )
    {
        yaw_index = -aiming_screen->negative_yaw_frame_count;
        yaw_fraction = 0.0f;
    }
    int16_t yaw_cell = yaw_index + aiming_screen->negative_yaw_frame_count;

    /* elevation -> pitch cell index + fraction */
    float pitch_delta = elevation >= 0.0f ? aiming_screen->positive_pitch_delta
                                          : aiming_screen->negative_pitch_delta;
    float pitch_t = pitch_delta == 0.0f ? 0.0f : elevation / pitch_delta;
    int16_t pitch_index = (int16_t)pitch_t;
    float pitch_fraction = (float)fmod(pitch_t, 1.0);
    if ( pitch_fraction < 0.0f )
    {
        pitch_fraction = pitch_fraction + 1.0f;
        pitch_index--;
    }
    if ( pitch_index >= aiming_screen->positive_pitch_frame_count )
    {
        pitch_fraction = 1.0f;
        pitch_index = aiming_screen->positive_pitch_frame_count - 1;
    }
    if ( pitch_index < -aiming_screen->negative_pitch_frame_count )
    {
        pitch_index = -aiming_screen->negative_pitch_frame_count;
        pitch_fraction = 0.0f;
    }
    int16_t pitch_cell = pitch_index + aiming_screen->negative_pitch_frame_count;

    if ( pitch_cell < 0 || pitch_cell >= grid_height || yaw_cell < 0 || yaw_cell >= grid_width )
        return;

    /* the four surrounding corner frames (clamped at the grid edge) */
    int16_t yaw_next = yaw_cell + 1 == grid_width ? yaw_cell : yaw_cell + 1;
    int16_t pitch_next = pitch_cell + 1 == grid_height ? pitch_cell : pitch_cell + 1;
    int16_t frame_index_00 = yaw_cell + pitch_cell * grid_width;
    int16_t frame_index_10 = yaw_next + pitch_cell * grid_width;
    int16_t frame_index_01 = yaw_cell + pitch_next * grid_width;
    int16_t frame_index_11 = yaw_next + pitch_next * grid_width;
    const char *frame_cursor_00 = animation_get_frame_data(animation, frame_index_00);
    const char *frame_cursor_10 = animation_get_frame_data(animation, frame_index_10);
    const char *frame_cursor_01 = animation_get_frame_data(animation, frame_index_01);
    const char *frame_cursor_11 = animation_get_frame_data(animation, frame_index_11);

    int16_t rotation_adjusted_index = 0;
    int16_t translation_adjusted_index = 0;
    unsigned int rotation_flags = 0;
    unsigned int translation_flags = 0;

    for ( int16_t node_index = 0; node_index < animation->node_count; node_index = (int16_t)(node_index + 1) )
    {
        real_orientation *node_orientation = &node_orientations[node_index];

        if ( (node_index & 0x1F) == 0 )
        {
            int word_index = node_index >> 5;
            translation_flags = animation->nodes_with_translation_flags[word_index];
            rotation_flags = animation->nodes_with_rotation_flags[word_index];
        }

        if ( rotation_flags & 1 )
        {
            real_quaternion rotation_00; /* var_100 */
            real_quaternion rotation_10; /* var_130 */
            real_quaternion rotation_01; /* var_120 */
            real_quaternion rotation_11; /* var_110 */
            if ( use_keyframe_accessors )
            {
                animation_get_keyframe_rotation(animation, (float)frame_index_00, rotation_adjusted_index,
                                                node_index, &rotation_00);
                animation_get_keyframe_rotation(animation, (float)frame_index_10, rotation_adjusted_index,
                                                node_index, &rotation_10);
                animation_get_keyframe_rotation(animation, (float)frame_index_01, rotation_adjusted_index,
                                                node_index, &rotation_01);
                animation_get_keyframe_rotation(animation, (float)frame_index_11, rotation_adjusted_index++,
                                                node_index, &rotation_11);
            }
            else
            {
                quaternion_decompress_8byte((const compressed_quaternion_8byte *)frame_cursor_00, &rotation_00);
                frame_cursor_00 += 8;
                quaternion_decompress_8byte((const compressed_quaternion_8byte *)frame_cursor_10, &rotation_10);
                frame_cursor_10 += 8;
                quaternion_decompress_8byte((const compressed_quaternion_8byte *)frame_cursor_01, &rotation_01);
                frame_cursor_01 += 8;
                quaternion_decompress_8byte((const compressed_quaternion_8byte *)frame_cursor_11, &rotation_11);
                frame_cursor_11 += 8;
            }

            real_quaternion rotation_pitch_cell_row; /* var_E0 — yaw blend along the pitch_cell row */
            real_quaternion rotation_pitch_next_row; /* var_F0 — yaw blend along the pitch_next row */
            real_quaternion blended_rotation;        /* var_D0 */
            quaternions_interpolate_and_normalize(&rotation_00, &rotation_10, yaw_fraction,
                                                  &rotation_pitch_cell_row);
            quaternions_interpolate_and_normalize(&rotation_01, &rotation_11, yaw_fraction,
                                                  &rotation_pitch_next_row);
            quaternions_interpolate_and_normalize(&rotation_pitch_cell_row, &rotation_pitch_next_row,
                                                  pitch_fraction, &blended_rotation);
            quaternions_multiply(&blended_rotation, &node_orientation->rotation, &node_orientation->rotation);
        }
        rotation_flags >>= 1;

        if ( translation_flags & 1 )
        {
            real_point3d translation_00; /* var_180 */
            real_point3d translation_10; /* var_1A0 */
            real_point3d translation_01; /* var_190 */
            real_point3d translation_11; /* var_1B0 */
            if ( use_keyframe_accessors )
            {
                animation_get_keyframe_translation(animation, (float)frame_index_00, translation_adjusted_index,
                                                   node_index, &translation_00);
                animation_get_keyframe_translation(animation, (float)frame_index_10, translation_adjusted_index,
                                                   node_index, &translation_10);
                animation_get_keyframe_translation(animation, (float)frame_index_01, translation_adjusted_index,
                                                   node_index, &translation_01);
                animation_get_keyframe_translation(animation, (float)frame_index_11, translation_adjusted_index++,
                                                   node_index, &translation_11);
            }
            else
            {
                translation_00 = *(const real_point3d *)frame_cursor_00;
                frame_cursor_00 += 12;
                translation_10 = *(const real_point3d *)frame_cursor_10;
                frame_cursor_10 += 12;
                translation_01 = *(const real_point3d *)frame_cursor_01;
                frame_cursor_01 += 12;
                translation_11 = *(const real_point3d *)frame_cursor_11;
                frame_cursor_11 += 12;
            }

            node_orientation->translation.__s1.x += (translation_01.__s1.x * (1.0f - yaw_fraction) + translation_11.__s1.x * yaw_fraction) * pitch_fraction
                                             + (translation_00.__s1.x * (1.0f - yaw_fraction) + translation_10.__s1.x * yaw_fraction) * (1.0f - pitch_fraction);
            node_orientation->translation.__s1.y += (translation_01.__s1.y * (1.0f - yaw_fraction) + translation_11.__s1.y * yaw_fraction) * pitch_fraction
                                             + (translation_00.__s1.y * (1.0f - yaw_fraction) + translation_10.__s1.y * yaw_fraction) * (1.0f - pitch_fraction);
            node_orientation->translation.__s1.z += (translation_00.__s1.z * (1.0f - yaw_fraction) + translation_10.__s1.z * yaw_fraction) * (1.0f - pitch_fraction)
                                             + (translation_01.__s1.z * (1.0f - yaw_fraction) + translation_11.__s1.z * yaw_fraction) * pitch_fraction;
        }
        translation_flags >>= 1;
    }
}
