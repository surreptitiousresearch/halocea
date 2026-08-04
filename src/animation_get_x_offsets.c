/* animation_get_x_offsets @0x83795728 — accumulate an animation's per-frame forward (x) movement to obtain the
 * cumulative x offset at the key frame and at the last frame. The per-frame movement layout depends on
 * frame_info_type (1 = dx only / stride 2, 2 = dx,dy,dyaw / stride 3, 3 = dx,dy,dz,dyaw / stride 4); the x delta
 * is read from the type-specific slot. frame_info.address is treated as one stride before the first frame (the
 * -8 bias), matching the engine's pre-increment walk. */

#include "headers/animation.h"
#include "headers/animation_frame_info_type.h"

void animation_get_x_offsets(animation *animation, float *key_frame_offset_reference, float *last_frame_offset_reference)
{
    int frame_count = animation->frame_count;
    float total_offset = 0.0f;
    float key_frame_offset = 0.0f;
    if ( frame_count > 0 )
    {
        int frame_info_type = (unsigned short)animation->frame_info_type;
        float *frame_info = (float *)((char *)animation->frame_info.address - 8);
        for ( int frame = 0; frame < frame_count; frame = (short)(frame + 1) )
        {
            float delta_x;
            if ( frame_info_type == _animation_frame_info_xy_translation )
            {
                frame_info += 2;
                delta_x = *frame_info;
            }
            else if ( frame_info_type == _animation_frame_info_xy_translation_yaw_rotation )
            {
                delta_x = frame_info[2];
                frame_info += 3;
            }
            else if ( frame_info_type == _animation_frame_info_xyz_translation_yaw_rotation )
            {
                delta_x = frame_info[2];
                frame_info += 4;
            }
            else
            {
                delta_x = 0.0f;
            }
            total_offset = delta_x + total_offset;
            if ( frame == animation->private_key_frame_index )
                key_frame_offset = total_offset;
        }
    }

    if ( last_frame_offset_reference )
        *last_frame_offset_reference = total_offset;
    if ( key_frame_offset_reference )
        *key_frame_offset_reference = key_frame_offset;
}
