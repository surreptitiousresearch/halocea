/* animation_second_key_frame_index @0x837956D0 — accessor for animation's private_second_key_frame_index. */

#include <stdint.h>
#include "headers/animation.h"

int16_t animation_second_key_frame_index(animation *animation)
{
    return animation->private_second_key_frame_index;
}
