/* animation_keyframe_search @0x83796310 — binary search over a sorted array of keyframe frame indices for the
 * keyframe bracketing target_frame_index: returns the largest index i such that keyframe_frame_indices[i] <=
 * target_frame_index and (i+1 == keyframe_count or keyframe_frame_indices[i+1] > target_frame_index). */

#include <stdint.h>

int16_t animation_keyframe_search(const int16_t *keyframe_frame_indices, int16_t keyframe_count, int16_t target_frame_index)
{
    int16_t low = 0;
    int16_t middle;
    for ( int16_t high = (int16_t)(keyframe_count - 1); ; high = middle )
    {
        while ( 1 )
        {
            middle = (int16_t)((high + low) >> 1);
            if ( middle + 1 >= keyframe_count || keyframe_frame_indices[middle + 1] > target_frame_index )
                break;
            low = middle;
        }
        if ( keyframe_frame_indices[middle] <= target_frame_index )
            break;
    }
    return middle;
}
