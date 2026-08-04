/* animation_left_foot_frame_index @0x837AC348 — accessor for animation's private_left_foot_frame_index. */

#include <stdint.h>
#include "headers/animation.h"

uint8_t animation_left_foot_frame_index(animation *animation)
{
    return animation->private_left_foot_frame_index;
}
