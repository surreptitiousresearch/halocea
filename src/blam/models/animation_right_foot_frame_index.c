/* animation_right_foot_frame_index @0x837AC350 — accessor for animation's private_right_foot_frame_index. */

#include <stdint.h>
#include "headers/animation.h"

uint8_t animation_right_foot_frame_index(animation *animation)
{
    return animation->private_right_foot_frame_index;
}
