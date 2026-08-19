/* animation_sound_frame_index @0x837956D8 — accessor for animation's private_sound_frame_index. */

#include <stdint.h>
#include "headers/animation.h"

int16_t animation_sound_frame_index(animation *animation)
{
    return animation->private_sound_frame_index;
}
