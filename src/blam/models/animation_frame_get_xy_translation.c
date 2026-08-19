/* animation_frame_get_xy_translation @0x837957D0 */
#include <stdint.h>
#include "headers/animation.h"
#include "headers/real_vector2d.h"
#include "headers/animation_frame_info_type.h"

void animation_frame_get_xy_translation(animation *animation, int16_t frame_index, real_vector2d *offset)
{
    if (animation->frame_info_type == _animation_frame_info_xy_translation)
    {
        *offset = ((real_vector2d *)animation->frame_info.address)[frame_index];
    }
    else
    {
        offset->n[0] = 0.0f;
        offset->n[1] = 0.0f;
    }
}
