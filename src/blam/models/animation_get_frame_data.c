/* animation_get_frame_data @0x838186F0 — pointer to the raw frame data for a given frame. When the animation
 * carries compressed data and global compression playback is enabled, all frames share the single compressed block
 * (at compressed_data_offset); otherwise frames are fixed-size records indexed by frame_index. */

#include <stdint.h>
#include "headers/animation.h"
#include "headers/animation_flags.h"
#include "headers/blam_data_globals.h"


void *animation_get_frame_data(const animation *animation, int16_t frame_index)
{
    int compressed = (animation->flags & (1u << _animation_compressed_bit)) != 0 && hs_model_animation_compression_enabled;
    int offset = compressed ? animation->compressed_data_offset
                            : animation->frame_size * frame_index;
    return (char *)animation->data.address + offset;
}
