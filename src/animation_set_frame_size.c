/* animation_set_frame_size @0x83795818 — compute an animation's per-frame byte size: 8 bytes for each node
 * with a rotation key, 12 for each with a translation key, 4 for each with a scale key. */

#include "headers/animation.h"

void animation_set_frame_size(animation *animation)
{
    __int16 frame_size = 0;

    for ( int node_index = 0; node_index < animation->node_count; ++node_index )
    {
        int word = node_index >> 5;
        unsigned int bit = 1u << (node_index & 0x1F);

        if ( animation->nodes_with_rotation_flags[word] & bit )
            frame_size += 8;
        if ( animation->nodes_with_translation_flags[word] & bit )
            frame_size += 12;
        if ( animation->nodes_with_scale_flags[word] & bit )
            frame_size += 4;
    }

    animation->frame_size = frame_size;
}
